

import os
import sys

execFileLocation = ("/Users/antoshkaplus/Documents"
                    "/Programming/Contests/TopCoder"
                    "/SquareRemover/DerivedData/SquareRemover"
                    "/Build/Products/Release/SquareRemover")

a_s = " "
for a in sys.argv[1:]:
    a_s += a + " "
os.system("java -jar SquareRemoverVis.jar -exec '" + execFileLocation + " -exec outer'" + a_s)