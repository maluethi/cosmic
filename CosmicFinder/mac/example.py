import sys
from ROOT import gSystem
gSystem.Load("libcosmics_CosmicFinder")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing CosmicFinder..."

sys.exit(0)

