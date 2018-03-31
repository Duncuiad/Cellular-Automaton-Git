#ifndef RULES_H_INCLUDED
#define RULES_H_INCLUDED

#include "grid.h"

/* Esempio: faccio la media di tutte le celle in un intorno quadrato di raggio dato */
Cell ruleAverage(const Grid *g, int x, int y, int radius);

Cell ruleCave(const Grid *g, int x, int y, int radius);

Cell ruleConvolve(const Grid *tgt, const Grid *op, int x, int y);

Cell ruleConway(const Grid *tgt, int x, int y);

void applyRuleAverage(Grid *g);

void applyRuleCave(Grid *g, int radius);

void applyRuleConvolve(Grid *tgt, const Grid *op);

void applyRuleNormalize(Grid *g, double tgtMin, double tgtMax);

void applyRuleSetMass(Grid *g, double tgtMass);

void slideshowRuleConvolve(Grid *g, const Grid *op, const char *filename);

void slideshowRuleConway(Grid *g, const char *filename);

void printGrid(const Grid *g);

#endif /*RULES_H_INCLUDED*/
