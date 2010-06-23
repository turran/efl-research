from utils import getPage, getText, getDirectDescendents
from text_elements import convertLine

from options import options

from templates.GroupsPage import GroupsPage

descriptions = {}
groups = []
flat = []

def registerGroup(group):
    l = [""]
    [x.getLines(l) for x in group.brief]
    descriptions[group.id] = "".join(l).strip()
    groups.append(group)

class DoxygenGroupsPage:
    def __init__(self):
        self.page = getPage("groups") 

    def addSub(self, parent, level):
        flat.append(parent)
        for sub in parent.groups:
            for g in groups:
                if g.id != sub.id:
                    continue
                g.level = level
                self.addSub(g, level + 1)

    def getFlat(self):
        return flat
    
    def getDescriptions(self):
        return descriptions

    def forest(self):
        roots = []
        for g1 in groups:
            is_root = True
            for g2 in groups:
                if g1 is g2:
                    continue
                if g1.id in [sub.id for sub in g2.groups]:
                    is_root = False
                    continue
            if is_root:
                g1.level = 0
                roots.append(g1)
        for root in roots:
            self.addSub(root, 1);
        
    def createFiles(self):
        self.forest()
        return [("wiki", self.page, GroupsPage(searchList={"summary": "A List of groups with brief descriptions.", "labels": options.labels, "prefix": options.prefix, "groups": flat, "descriptions": descriptions}))]
