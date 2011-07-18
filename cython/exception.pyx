cdef class PyException:
    cdef Exception *_thisptr
    def __cinit__(self, char* when, char* msg):
        cdef string *swhen = new string(when)
        cdef string *smsg = new string(msg)
        self._thisptr = new Exception(swhen[0], smsg[0])
        del swhen
        del smsg
    def __dealloc__(self):
        del self._thisptr

    def description(self):
        cdef string str
        str  = self._thisptr.description()
        return str.c_str()
        
    def when(self):
        cdef string str = self._thisptr.when()
        return str.c_str()