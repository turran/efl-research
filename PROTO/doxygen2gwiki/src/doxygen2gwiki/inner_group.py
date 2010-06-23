from utils import getPage, getText, camelCase

from templates.Function import Function

class DoxygenInnerGroup:
    def __init__(self, xml):
        self.id = xml.attributes["refid"].value
        self.name = getText(xml.childNodes)
        self.page = getPage(self.id)
