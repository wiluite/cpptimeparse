import sys

sys.path.append("build/")

from cpptimeparse import time_parser
from decimal import Decimal

Parser = time_parser()
if Parser.parse(" 2d, 1hr, 13 mins  02.1667891s ") == True:
	print("Print formatted time with nanoseconds:")
	print(Parser.str(time_parser.NANO))

	print("Print formatted time from triple (seconds, microseconds, nanoseconds):")
	time_triple = Parser.time_tuple()
	print(time_triple)
	print(Parser.str(time_triple[0] + time_triple[1]/1000000 + time_triple[2]/1000000000,  time_parser.NANO))

	print("Print formatted time <seconds.microseconds> converted to double:")
	print(Parser.str(Decimal(Parser.time()), time_parser.NANO))

print("Print from any double (with microseconds):")
print(Parser.str(177182.1667899, time_parser.MICRO))
print("Print from any double (with nanoseconds):")
print(Parser.str(177182.1667899, time_parser.NANO))

print("days=", Parser.days())
print("clock_hours=", Parser.clock_hours())
print("clock_minutes=", Parser.clock_minutes())
print("clock_seconds=", Parser.clock_seconds())
print("microseconds=", Parser.microseconds())
print("nanoseconds=", Parser.nanoseconds())
