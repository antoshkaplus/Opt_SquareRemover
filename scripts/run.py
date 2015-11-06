import sys
import os

command = ("java -jar ./SquareRemoverVis.jar -exec ./../bin/square_remover_app "
                + " ".join(sys.argv[1:] if len(sys.argv) > 1 else [""]))

os.system(command)