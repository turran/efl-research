from utils import getText, getPage, getDirectDescendents, camelCase

from options import options
from member_function import DoxygenMemberFunction
from inner_group import DoxygenInnerGroup
from text_elements import convertLine
from groupspage import registerGroup
from doxygen import doxygen
from templates.Group import Group

class DoxygenGroup:
    def __init__(self, xml):
        self.id = xml.attributes["id"].value
        self.name = getText(xml.getElementsByTagName("title")[0].childNodes)
        self.page = getPage(self.id)

        self.brief = convertLine(getDirectDescendents(xml, "briefdescription")[0], self)
        self.detailed = convertLine(getDirectDescendents(xml, "detaileddescription")[0], self)

        registerGroup(self)

        funcs = [n for n in getDirectDescendents(xml, "sectiondef") if n.getAttribute("kind") == "func"]
        if len(funcs) > 0:
            self.functions = [DoxygenMemberFunction(x, self) for x in getDirectDescendents(funcs[0], "memberdef")]
        else:
            self.functions = []
        
        groups = [n for n in getDirectDescendents(xml, "innergroup")]
        if len(groups) > 0:
            self.groups = [DoxygenInnerGroup(x) for x in groups]
        else:
            self.groups = []
        
        typedefs = [n for n in getDirectDescendents(xml, "sectiondef") if n.getAttribute("kind") == "typedef"]
        if len(typedefs) > 0:
            self.typedefs = [DoxygenMemberFunction(x, self) for x in getDirectDescendents(typedefs[0], "memberdef")]
        else:
            self.typedefs = []

    def createFiles(self):
        brief = [""]
        for b in self.brief:
            b.getLines(brief)
        detailed = [""]
        for b in self.detailed:
            b.getLines(detailed)
        return [("wiki", self.page, Group(searchList={"summary": "Documentation for the %s group" % (self.name, ), "ref": self.id, "labels": options.labels, "prefix": options.prefix, "filename": self.name, "briefdescription": "".join(brief).strip(), "detaileddescription": "".join(detailed).strip(), "functions": self.functions, "groups": self.groups, "typedefs": self.typedefs}))]


