# Script to check that the length of a line does not exceed 79 characters
# Commented out for now
import sys
input = sys.stdin.readline

exitCode = 0
# for i in range(1, len(sys.argv)):
#   filename = sys.argv[i]
#   with open(filename, "r") as file:
#     replacePragmaOnce = False
#     for line in file:
#       if line[-1] == '\n':
#         line = line[:-1]
#       if len(line) > 79:
#         print(filename + " violates maximum line length of 79")
#         exitCode = 1
#         break
sys.exit(exitCode)