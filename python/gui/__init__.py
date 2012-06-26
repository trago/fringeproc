import sip
api2_classes = [
    'QData', 'QDateTime', 'QString', 'QTextStream',
    'QTime', 'QUrl', 'QVariant']
for cl in api2_classes:
    sip.setapi(cl, 2)

__all__=['fringeproc']
