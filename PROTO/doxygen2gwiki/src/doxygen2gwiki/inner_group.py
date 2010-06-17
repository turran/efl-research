from utils import getText, camelCase

from templates.Function import Function

class DoxygenInnerGroup:
    def __init__(self, xml):
        self.id = camelCase(xml.attributes["refid"].value)
        self.name = getText(xml.childNodes)
