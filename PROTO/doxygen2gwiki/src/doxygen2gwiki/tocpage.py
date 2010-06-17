from templates.Toc import Toc
from groupspage import DoxygenGroupsPage

from options import options

class DoxygenTocPage:
    def __init__(self):
        pass

    def createFiles(self):
        groups = DoxygenGroupsPage().getFlat()
        groups_descriptions = DoxygenGroupsPage().getDescriptions()
        return [("wiki", options.prefix + "_toc", Toc(searchList={"labels": options.labels, "prefix": options.prefix, "groups": groups, "groups_descriptions": groups_descriptions}))]

from utils import getText

