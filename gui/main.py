import sys

from PySide6.QtCore import QObject, Slot
from PySide6.QtGui import QGuiApplication
from PySide6.QtQml import QQmlApplicationEngine, QmlElement
from PySide6.QtQuickControls2 import QQuickStyle

QML_IMPORT_NAME = "spellbook"
QML_IMPORT_MAJOR_VERSION = 1

@QmlElement
class Bridge(QObject):
    @Slot(str)
    def validateInput(self, input):
        print(input)

if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()
    QQuickStyle.setStyle("Material")
    engine.addImportPath(sys.path[0])
    engine.loadFromModule("SpellBook", "Main")
    if not engine.rootObjects():
        sys.exit(-1)
    exit_code = app.exec()
    del engine
    sys.exit(exit_code)