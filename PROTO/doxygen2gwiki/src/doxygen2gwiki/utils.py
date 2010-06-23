import re
from options import options
from string import capitalize

def getPage(id):
    print "%s %s %s" % (options.prefix, id, camelCase(options.prefix + id))
    return camelCase(options.prefix + "_" + id)

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
