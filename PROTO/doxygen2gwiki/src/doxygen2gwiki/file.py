from utils import getText, getPage, getDirectDescendents, camelCase

from options import options
from member_function import DoxygenMemberFunction
from text_elements import convertLine, ProgramListing
from filespage import registerFileBriefDescription

from templates.FilePage import FilePage

class DoxygenFile:
    def __init__(self, xml):
        self.id = xml.attributes["id"].value
        self.name = getText(xml.getElementsByTagName("compoundname")[0].childNodes)
        self.page = getPage(self.id)
        self.brief = convertLine(getDirectDescendents(xml, "briefdescription")[0], self)
        self.detailed = convertLine(getDirectDescendents(xml, "detaileddescription")[0], self)

        l = [""]
        [x.getLines(l) for x in self.brief]
        registerFileBriefDescription(self.id, "".join(l).strip())

        funcs = [n for n in getDirectDescendents(xml, "sectiondef") if n.getAttribute("kind") == "func"]
        if len(funcs) > 0:
            self.functions = [DoxygenMemberFunction(x, self) for x in getDirectDescendents(funcs[0], "memberdef")]
        else:
            self.functions = []
        
        typedefs = [n for n in getDirectDescendents(xml, "sectiondef") if n.getAttribute("kind") == "typedef"]
        if len(typedefs) > 0:
            self.typedefs = [DoxygenMemberFunction(x, self) for x in getDirectDescendents(typedefs[0], "memberdef")]
        else:
            self.typedefs = []

        print "typedefs = %s" % len(typedefs)
        self.programlisting = ProgramListing(xml.getElementsByTagName("programlisting")[0], self)

    def createFiles(self):
        brief = [""]
        for b in self.brief:
            b.getLines(brief)
        detailed = [""]
        for b in self.detailed:
            b.getLines(detailed)
        return [("wiki", self.page, FilePage(searchList={"summary": "Documentation for the %s file" % (self.name, ), "labels": options.labels, "prefix": options.prefix, "filename": self.name, "briefdescription": "".join(brief).strip(), "detaileddescription": "".join(detailed).strip(), "functions": self.functions, "typedefs": self.typedefs}))]

