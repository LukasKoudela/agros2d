// *************************************************************************
//
//    PARALUTION   www.paralution.com
//
//    Copyright (C) 2012-2013 Dimitar Lukarski
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef PARALUTION_OCL_MATRIX_COO_HPP_
#define PARALUTION_OCL_MATRIX_COO_HPP_

#include "../base_matrix.hpp"
#include "../base_vector.hpp"
#include "../matrix_formats.hpp"

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


namespace paralution {

template <class ValueType>
class OCLAcceleratorMatrixCOO : public OCLAcceleratorMatrix<ValueType> {
  
public:

  OCLAcceleratorMatrixCOO();
  OCLAcceleratorMatrixCOO(const Paralution_Backend_Descriptor local_backend);
  virtual ~OCLAcceleratorMatrixCOO();

  virtual void info(void) const;
  virtual unsigned int get_mat_format(void) const{ return COO; }

  virtual void Clear(void);
  virtual void AllocateCOO(const int nnz, const int nrow, const int ncol);

  virtual bool ConvertFrom(const BaseMatrix<ValueType> &mat);

  virtual bool Permute(const BaseVector<int> &permutation);
  virtual bool PermuteBackward(const BaseVector<int> &permutation);

  virtual void CopyFrom(const BaseMatrix<ValueType> &mat);
  virtual void CopyTo(BaseMatrix<ValueType> *mat) const;

  virtual void CopyFromHost(const HostMatrix<ValueType> &src);
  virtual void CopyToHost(HostMatrix<ValueType> *dst) const;

  virtual void Apply(const BaseVector<ValueType> &in, BaseVector<ValueType> *out) const; 
  virtual void ApplyAdd(const BaseVector<ValueType> &in, const ValueType scalar,
                        BaseVector<ValueType> *out) const; 
  
private:
  
  MatrixCOO<cl_mem, cl_mem> mat_;

  friend class BaseVector<ValueType>;  
  friend class AcceleratorVector<ValueType>;  
  friend class OCLAcceleratorVector<ValueType>;  

};


}

#endif // PARALUTION_OCL_MATRIX_COO_HPP_

