/*--------------------------------------------------------------------------
@COPYRIGHT  :
              Copyright 1996, Alex P. Zijdenbos, 
              McConnell Brain Imaging Centre,
              Montreal Neurological Institute, McGill University.
              Permission to use, copy, modify, and distribute this
              software and its documentation for any purpose and without
              fee is hereby granted, provided that the above copyright
              notice appear in all copies.  The author and McGill University
              make no representations about the suitability of this
              software for any purpose.  It is provided "as is" without
              express or implied warranty.
---------------------------------------------------------------------------- 
$RCSfile: Histogram.h,v $
$Revision: 1.1.1.1 $
$Author: jason $
$Date: 2001-11-09 16:37:25 $
$State: Exp $
--------------------------------------------------------------------------*/
#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <iostream.h>
#include "MTypes.h"
#include "ValueMap.h"
#include "SimpleArray.h"

class Histogram : private SimpleArray<unsigned> {
  double    _min, _max; // True extrema (i.e., not the bin centers)
  double    _binWidth;
  LinearMap _valueToBinMap;

public:
// Constructors/destructor
  // The min and max values in these constructors are bin centers,
  // not absolute extrema.
  Histogram();
  Histogram(double min, double max, unsigned nBins = 0);
  Histogram(double min, double max, double binWidth);
  Histogram(unsigned nBins, double min = 0.0, double binWidth = 1.0);
  Histogram(const Histogram&);
  virtual ~Histogram() {}

  Histogram& operator = (const Histogram&);
  Histogram& newRange(double min, double max); // Changes ranges; keeps binWidth

// Get functions
  unsigned  operator [] (unsigned i) const { 
    return SimpleArray<unsigned>::operator [] (i); }
  unsigned& operator [] (unsigned i) { 
    return SimpleArray<unsigned>::operator [] (i); }
  double    binWidth() const            { return _binWidth; }
  unsigned  nBins() const               { return _size; }
  double    binStart(unsigned i) const  { return _min + i*_binWidth; }
  DblArray  binStarts() const;
  double    binCenter(unsigned i) const { return binStart(i) + _binWidth/2; }
  DblArray  binCenters() const          { return binStarts() + _binWidth/2; }
  unsigned  n() const                   { return unsigned(sum()); }
  unsigned  count(double value) const   { return _contents[bin(value)]; }
  unsigned  bin(double value) const;
  unsigned  max(unsigned *bin = 0) const;
  double    mean() const;
  double    median(unsigned *bin = 0, unsigned nBelow = 0, unsigned nAbove = 0) const;
  double    majority(unsigned *bin = 0) const;
  double    biModalThreshold() const;
  double    varianceThreshold() const;
  double    kullbackThreshold() const;
  double    pctThreshold(double pct) const;
  double    entropy() const;
  DblArray  pdf() const;
  DblArray  cdf() const;

// Set functions
  Boolean add(double value) {
    if ((value < _min) || (value > _max))
      return FALSE;
    unsigned index = (unsigned) _valueToBinMap(value);
    if (index >= _size)
      index = _size - 1;
    _contents[index]++;
    return TRUE;
  }

// Other operators
  Histogram&  operator += (const Histogram& hist);
  LUT<double> equalize(const Histogram& hist) const;

// I/O
  ostream& printHeadAndTail(ostream& os, unsigned n = 10) const;
#ifdef HAVE_MATLAB
  Boolean  saveMatlab(const char *fileName, const char *binVarName = "X", 
		      const char *countVarName = "N", const char *option = "u") const;
#endif

// Friends
  friend ostream&    operator << (ostream& os, const Histogram& hist);
  friend DblArray    pdf(const Histogram& hist) { return hist.pdf(); }
  friend DblArray    cdf(const Histogram& hist) { return hist.cdf(); }
  friend LUT<double> equalize(const Histogram& hist1, const Histogram& hist2) {
    return hist1.equalize(hist2); }

  friend SimpleArray<double> asDblArray(const Histogram& hist);
};

#endif