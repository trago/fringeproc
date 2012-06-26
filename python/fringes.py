import sys
from gui.fringeproc import FringeProc

def main(argv):
    fringeproc = FringeProc(argv)
    fringeproc.start()
    
if __name__=='__main__':
    main(sys.argv)
