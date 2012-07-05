import sys
from gui.fringeproc import FringeProc

def main(argv):
    fringe = FringeProc(argv)
    fringe.start()
    
if __name__=='__main__':
    main(sys.argv)
