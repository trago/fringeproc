cdef extern from "<string>" namespace "std":
    cdef cppclass string:
        string()
        string(char*)
        char* c_str()
    
cdef extern from "exception.h":
    cdef cppclass Exception:
        Exception(string, string)
        string description()
        string when()
