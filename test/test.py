
import os
import sys

execFileLocation = ("/Users/antoshkaplus/Documents"
                    "/Programming/Contests/TopCoder"
                    "/SquareRemover/DerivedData/SquareRemover"
                    "/Build/Products/Release/SquareRemover")

print " ".join(sys.argv[1:])

os.system(execFileLocation + " -exec test " + " ".join(sys.argv[1:]))