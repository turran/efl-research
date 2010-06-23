from utils import getPage, getText

from options import options

from templates.GlobalsPage import GlobalsPage

members = []

def registerGlobals(xml):
    filename = getText(xml.getElementsByTagName("name")[0].childNodes)
    for f in xml.getElementsByTagName("member"):
        doxygen.footer["globals"] = True
        members.append((f.attributes["refid"].value, getText(f.getElementsByTagName("name")[0].childNodes), (xml.attributes["refid"].value, filename)))

class DoxygenGlobalsPage:
    def __init__(self):
        self.page = getPage("globals")
        pass

    def createFiles(self):
        return [("wiki", self.page, GlobalsPage(searchList={"summary": "A list of all functions, variables, defines, enums, and typedefs with links to the files they belong to.", "labels": options.labels, "prefix": options.prefix, "members": members}))]

from utils import getText
from doxygen import doxygen
