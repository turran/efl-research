from utils import getText, getDirectDescendents
from text_elements import convertLine

from options import options

from templates.GroupsPage import GroupsPage

descriptions = {}
groups = []

def registerGroup(group):
    l = [""]
    [x.getLines(l) for x in group.brief]
    descriptions[group.id] = "".join(l).strip()
    groups.append(group)

class DoxygenGroupsPage:
    def __init__(self):
        pass

    def createFiles(self):
        return [("wiki", options.prefix + "_groups", GroupsPage(searchList={"summary": "A List of groups with brief descriptions.", "labels": options.labels, "prefix": options.prefix, "groups": groups, "descriptions": descriptions}))]

from utils import getText
