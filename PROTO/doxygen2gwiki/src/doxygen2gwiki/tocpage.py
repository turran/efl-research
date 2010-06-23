from utils import getPage, getText
from templates.Toc import Toc
from groupspage import DoxygenGroupsPage

from options import options

class DoxygenTocPage:
    def __init__(self):
        self.page = getPage("toc")
        pass

    def createFiles(self):
        groups = DoxygenGroupsPage().getFlat()
        groups_descriptions = DoxygenGroupsPage().getDescriptions()
        return [("wiki", self.page, Toc(searchList={"labels": options.labels, "prefix": options.prefix, "groups": groups, "groups_descriptions": groups_descriptions}))]

