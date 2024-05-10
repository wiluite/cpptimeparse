import sys

sys.path.append("build/")

from cpptimeparse import time_parser
from decimal import Decimal

Parser = time_parser()
if Parser.parse(" 2d, 1hr, 13 mins  02.166s ") == True:
	# print formatted time
	print(Parser.str())

	# print formatted time from string result converted to double
	print(Parser.str(Decimal(Parser.time())))

	# print formatted time from seconds and microseconds result
	time_pair = Parser.time_pair()
	print(Parser.str(time_pair[0] + time_pair[1]/1000000))

# print from a double
print(Parser.str(177182.166))

print("days=", Parser.days())
print("clock_hours=", Parser.clock_hours())
print("clock_minutes=", Parser.clock_minutes())
print("clock_seconds=", Parser.clock_seconds())
print("microseconds=", Parser.microseconds())
