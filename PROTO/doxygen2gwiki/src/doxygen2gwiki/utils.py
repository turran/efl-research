import re
from string import capitalize

def camelCase(value):
    return "".join([capitalize(w) for w in re.split(re.compile("[\W_]*"), value)])

def getText(nodelist):
    rc = ""
    for node in nodelist:
        if node.nodeType == node.TEXT_NODE:
            rc = rc + node.data
    return rc

def getDirectDescendents(node, tagname):
    return [n for n in node.getElementsByTagName(tagname) if n.parentNode is node]
