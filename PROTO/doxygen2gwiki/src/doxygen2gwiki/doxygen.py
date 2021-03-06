import os
from xml.dom.minidom import parseString

from options import options
from utils import getText
from fix_xml import fixXML

class Doxygen:
    def __init__(self):
        self.files = []
        self.staticfiles = []
        self.links = {}
        self.footer = {}
        if options.no_labels:
            self.labels = []
        elif options.labels == []:
            self.labels = ["Doxygen"]
        else:
            self.labels = options.labels

    def processFile(self, xml):
        if xml.documentElement.tagName == "doxygenindex":
            pages = [node.attributes["refid"].value for node in xml.documentElement.getElementsByTagName("compound") if node.attributes["kind"].value == "page"]
            for f in pages:
                if options.verbose:
                    print "Processing", f + ".xml"
                self.processFile(parseString(fixXML(open(options.docs + f + ".xml", "r").read())))
            files = [node.attributes["refid"].value for node in xml.documentElement.getElementsByTagName("compound") if node.attributes["kind"].value == "file"]
            for f in files:
                if options.verbose:
                    print "Processing", f + ".xml"
                self.processFile(parseString(fixXML(open(options.docs + f + ".xml", "r").read())))
            dirs = [node.attributes["refid"].value for node in xml.documentElement.getElementsByTagName("compound") if node.attributes["kind"].value == "dir"]
            for f in dirs:
                if options.verbose:
                    print "Processing", f + ".xml"
                self.processFile(parseString(fixXML(open(options.docs + f + ".xml", "r").read())))
            groups = [node.attributes["refid"].value for node in xml.documentElement.getElementsByTagName("compound") if node.attributes["kind"].value == "group"]
            for f in groups:
                if options.verbose:
                    print "Processing", f + ".xml"
                self.processFile(parseString(fixXML(open(options.docs + f + ".xml", "r").read())))
            for c in xml.documentElement.getElementsByTagName("compound"):
                registerGlobals(c)
        elif xml.documentElement.tagName == "doxygen":
            compounds = xml.documentElement.getElementsByTagName("compounddef")
            for c in compounds:
                if c.attributes["kind"].value == "file":
                    self.footer["file"] = True
                    self.files.append(DoxygenFile(c))
                elif c.attributes["kind"].value == "page":
                    self.files.append(DoxygenPage(c))
                elif c.attributes["kind"].value == "dir":
                    registerDir(c)
                elif c.attributes["kind"].value == "group":
                    self.footer["group"] = True
                    self.files.append(DoxygenGroup(c))
                else:
                    raise SystemError, "Unrecognised compound type. (%s)" % (c.attributes["kind"].value, )
        else:
            raise SystemError, "Unrecognised root file node. (%s)" % (xml.documentElement.tagName, )

    def addLink(self, refid, page, anchor):
        self.links[refid] = DoxygenLink(page, anchor)

    def getLink(self, id):
        try:
            return self.links[id]
        except KeyError:
            return None

    def createFiles(self):
        files = []
        for f in self.files:
            files += f.createFiles()
        if self.footer.has_key("file"):
            files += DoxygenFilesPage().createFiles()
        if self.footer.has_key("globals"):
            files += DoxygenGlobalsPage().createFiles()
        if self.footer.has_key("group"):
            files += DoxygenGroupsPage().createFiles()
        files += DoxygenTocPage().createFiles()
        return files + self.staticfiles

    def copyFile(self, type, _from, _to):
        if options.output + _to not in [x[1] for x in self.staticfiles]:
            open(options.output + _to, "wb").write(open(_from, "rb").read())
            self.staticfiles.append(("static", _to, None))

doxygen = Doxygen()

from link import DoxygenLink
from page import DoxygenPage
from file import DoxygenFile
from group import DoxygenGroup
from groupspage import DoxygenGroupsPage
from tocpage import DoxygenTocPage
from filespage import registerDir, DoxygenFilesPage
from globalspage import registerGlobals, DoxygenGlobalsPage
from member_function import DoxygenMemberFunction
