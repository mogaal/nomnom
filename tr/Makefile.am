qmfiles=	NomNom_fi_FI.qm NomNom_de_DE.qm
EXTRA_DIST=	NomNom_fi_FI.ts NomNom_de_DE.ts
CLEANFILES=	$(qmfiles)

OPTS=	-compress
OPTS+=	-removeidentical
OPTS+=	-nounfinished

NomNom_fi_FI.qm:
	$(LRELEASE) $(OPTS) -qm $@ $(top_srcdir)/tr/NomNom_fi_FI.ts

NomNom_de_DE.qm:
	$(LRELEASE) $(OPTS) -qm $@ $(top_srcdir)/tr/NomNom_de_DE.ts

qmdir=	$(datarootdir)/nomnom/tr
qm_DATA=$(qmfiles)
