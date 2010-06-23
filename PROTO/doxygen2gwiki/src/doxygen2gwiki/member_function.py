from utils import getText

from templates.Function import Function
from doxygen import doxygen
from text_elements import convertLine

class DoxygenMemberFunction:
    def __init__(self, xml, parent):
        try:
            self.refid = xml.attributes["refid"].value
        except KeyError:
            self.refid = xml.attributes["id"].value

        self.name = getText(xml.getElementsByTagName("name")[0].childNodes)

        l = [""]
        [x.getLines(l) for x in convertLine(xml.getElementsByTagName("briefdescription")[0], self)]
        self.brief = "".join(l).strip()

        l = [""]
        [x.getLines(l) for x in convertLine(xml.getElementsByTagName("detaileddescription")[0], self)]
        self.detailed = "".join(l).strip()

        self.link = self.brief.replace(' ', '_')
        doxygen.addLink(self.refid, parent.page, self.link)
        
        l = [""]
        [x.getLines(l) for x in convertLine(xml.getElementsByTagName("type")[0], self)]
        self.type = "".join(l).strip()
        print "type ==== %s" % self.type
        self.argsstring = getText(xml.getElementsByTagName("argsstring")[0].childNodes)
        self.params = []
        # detailed description -> parameterlist -> parametername
        # simplesect (return)
        for p in xml.getElementsByTagName("param"):
            type = getText(p.getElementsByTagName("type")[0].childNodes)
            declname = p.getElementsByTagName("declname")
            if len(declname) > 0:
               self.params.append((type, getText(declname[0].childNodes)))
        self.type = getText(xml.getElementsByTagName("type")[0].childNodes)

    def __get_sig(self):
        return self.type + " " + self.name + "(" + ", ".join(["%s %s" % x for x in self.params]) + ")"
    sig = property(__get_sig)

    def __get_doc(self):
        return unicode(Function(searchList={"f": self}))
    doc = property(__get_doc)

