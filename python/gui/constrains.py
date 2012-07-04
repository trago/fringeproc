from PyQt4.QtCore import QObject,  pyqtSignal

class Constrains(QObject):
    """
    Manages constrains for the actions presented in the user
    interface.

    There is a set of constrains and a set of current constrains that
    here we refer as state. We can set the constrains as a single
    constrain or by passing a lis of constrains. If we set a constrain
    that is not registered here, a `KeyError` exception is raised.

       :attribute init_state: Initial state.
       :attribute data_loaded: The data has been loaded.
       :attribute data_unloaded: The data has been unloaded.
       :attribute data_saved: The data has been saved.
       :attribute data_processing: The data is being processed.
       :attribute data_processed: The data has been processed.
       :attribute data_invalid: The data is invalid.
       :attribute action_canceled: The action was canceled.
       :attribute action_executed: The action was excetude.
       :attribute action_accepted: The action was accepted.
       :attribute file_open: The file is open.
       :attribute file_closed: The file is close.
       :attribute file_saved: The file is saved.
       :attribute user_interacting: The user is interaction.
       :attribute busy_state: The application is busy.

       :attribute _constrain: The set of registered constrains

       :author: Julio C. Estrada <julio@cio.mx>
    """

    init_state = 0 # Initial state
    data_loaded = 1 # The data has been loaded
    data_unloaded = 2 # The data has been unloaded
    data_saved = 3 # The data has been saved
    data_processing = 4 # The data is being processed
    data_processed = 5 # The data has been processed
    data_invalid = 6 # The data is invalid

    action_canceled = 7 # The action was canceled
    action_executed = 8 # The action was excetude
    action_accepted = 9 # The action was accepted

    file_open = 10 # The file is open
    file_closed = 11 # The file is close
    file_saved = 12 # The file is saved

    user_interacting = 13 # The user is interaction

    busy_state = 14 # The application is busy

    # The set of constrains.
    _constrains = set([init_state, data_loaded, data_saved, data_unloaded,
                       data_processing, data_processed, data_processed,
                       action_accepted, action_canceled, action_executed,
                       file_closed, file_open, file_saved,
                       user_interacting, busy_state])
    # The current state
    _state = set([init_state])

    # Signal to notify that the state has been changed
    stateChanged = pyqtSignal(set)

    def __init__(self, parent=None):
        """
        Initializes the QObject.

           :author: Julio C. Estrada <julio@cio.mx>
        """
        super(Constrains, self).__init__(parent)


    def setState(self, uistate):
        """
        Sets the current state.

        The current state can be set as a single constrain or a list
        of constrains. 

        :param uistate: The constrain or constrains to be set in the
                        current state.  
        :type uistate: int or str. 
        :raises:
              - KeyError if the constrains given are not registered in
                `Constrains` 
              - TypeError if is not passed an integer or a list of
                integers 

        :author: Julio C. Estrada <julio@cio.mx>
        """
        if isinstance(uistate, list):
            state = set(uistate)
        elif isinstance(uistate, int):
            state = set([uistate])
        else:
            raise TypeError, "Argument must be int or list"

        if len(state & self._constrains) > 0:
            self._state = state
        else:
            raise KeyError, "Attemptinng to set an unknown state"

        self.stateChanged.emit(state)

    def getState(self):
        """
        Gives the current state.

        :returns: int or list, the constrain or list of constrains
                  of the current state.
          :author: Julio C. Estrada <julio@cio.mx>
        """
        return self._state

    def All(self):
        """
        Gives the set of all registered constrains.

        :returns: set, the set of all constrains.
        :author: Julio C. Estrada <julio@cio.mx>
        """
        return Constrains._constrains.copy()
