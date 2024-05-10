#include <pybind11/pybind11.h>
#include "timeparse.h"

namespace py = pybind11;

PYBIND11_MODULE(cpptimeparse, m) {
    m.doc() = "Pybind11 wrapper for Python's pytimeparse module's analog, implemented in C++ with no regular expressions.";
    py::class_<time_parser>(m, "time_parser")
            .def(py::init<>())
            .def("parse", &time_parser::parse, "Parses the argument expression.")
            .def("time", &time_parser::time, "Returns time string in format: <seconds.microseconds>")
            .def("time_pair", &time_parser::time_pair, "Returns digital time pair: (seconds, microseconds)")
            .def("str", static_cast<std::string (time_parser::*)() const>(&time_parser::str), "Returns the latest parse result.")
            .def("str", static_cast<std::string (time_parser::*)(long double) const >(&time_parser::str), "Returns result by argument passed.")
            .def("days", &time_parser::days, "Returns the latest parse result's days.")
            .def("clock_hours", &time_parser::clock_hours, "Returns the latest parse result's clock_hours.")
            .def("clock_minutes", &time_parser::clock_minutes, "Returns the latest parse result's clock_minutes.")
            .def("clock_seconds", &time_parser::clock_seconds, "Returns the latest parse result's clock_seconds.")
            .def("microseconds", &time_parser::microseconds, "Returns the latest parse result's microseconds.")
            ;
}
