#include <pybind11/pybind11.h>
#include "timeparse.h"

namespace py = pybind11;

PYBIND11_MODULE(cpptimeparse, m) {
    m.doc() = "Pybind11 wrapper for Python's pytimeparse module's analog, implemented in C++ with no regular expressions.";
    py::class_<time_parser> p(m, "time_parser");
    p.def(py::init<>())
            .def("parse", &time_parser::parse, "Parses the argument expression.")
            .def("time", &time_parser::time, "Returns time string: seconds.microseconds")
            .def("time_tuple", &time_parser::time_tuple, "Returns integer time triple: (seconds, microseconds, nenoseconds)")
            .def("str", static_cast<std::string (time_parser::*)(time_parser::output_resolution) const>(&time_parser::str)
                 , "Returns the latest parse result according to csvkit's TimeDelta format.")
            .def("str", static_cast<std::string (time_parser::*)(long double, time_parser::output_resolution) const >(&time_parser::str)
                 , "Returns the above but by argument passed.")
            .def("days", &time_parser::days, "Returns the latest parse days.")
            .def("clock_hours", &time_parser::clock_hours, "Returns the latest parse clock_hours.")
            .def("clock_minutes", &time_parser::clock_minutes, "Returns the latest parse clock_minutes.")
            .def("clock_seconds", &time_parser::clock_seconds, "Returns the latest parse clock_seconds.")
            .def("microseconds", &time_parser::microseconds, "Returns the latest parse microseconds.")
            .def("nanoseconds", &time_parser::nanoseconds, "Returns the latest parse nanoseconds.");

    py::enum_<time_parser::output_resolution>(p, "output_resolution")
            .value("MICRO", time_parser::output_resolution::MICRO)
            .value("NANO", time_parser::output_resolution::NANO)
            .export_values();
}
