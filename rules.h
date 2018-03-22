#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED

#include "grid.h"

/* Esempio: faccio la media di tutte le celle in un intorno quadrato di raggio dato */
Cell ruleAverage(Grid g, int x, int y, int radius);


Cell ruleConvolve(Grid tgt, Grid op, int x, int y);

Cell ruleConway(Grid tgt, int x, int y);

void applyRuleConvolve(Grid *tgt, Grid op);

void applyRuleNormalize(Grid *g, double tgtMin, double tgtMax);

void applyRuleSetMass(Grid *g, double tgtMass);

void initRuleConway(Grid *tgt);

void slideshowRuleConvolve(Grid *g, Grid op, const char *filename);

void slideshowRuleConway(Grid *g, const char *filename);

#endif /*RULES_H_INCLUDED*/
