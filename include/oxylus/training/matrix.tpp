/*
 * Copyright (C) 2017
 * SIPLab Tecnológico de Costa Rica
 *
 * \author: Pablo Alvarado
 * \date:   28.12.2017
 */

namespace rdf
{

  // -------------------------------------------
  // Implementation of Matrix::_Matrix_impl
  // -------------------------------------------

  template<typename T>
  Matrix<T>::_Matrix_impl::_Matrix_impl()
    : _data(), _rows(), _cols() { }

  template<typename T>
  void Matrix<T>::_Matrix_impl::
  _swap_data(_Matrix_impl& _x) noexcept {
    std::swap(_data,  _x._data);
    std::swap(_rows,  _x._rows);
    std::swap(_cols,  _x._cols);
  }

  // ------------------------
  // Some inliners
  // ------------------------

  template <typename T>
  inline typename Matrix<T>::iterator Matrix<T>::begin() {
    return this->_impl._data;
  }

  template <typename T>
  inline typename Matrix<T>::const_iterator Matrix<T>::begin() const {
    return this->_impl._data;
  }

  template <typename T>
  inline typename Matrix<T>::iterator Matrix<T>::end() {
    return this->_impl._data + _impl.tentries();
  }

  template <typename T>
  inline typename Matrix<T>::const_iterator Matrix<T>::end() const {
    return this->_impl._data + _impl.tentries();
  }

  // ------------------------
  // Implementation of Matrix
  // ------------------------

  template<typename T>
  Matrix<T>::Matrix() : _impl() {}

  template<typename T>
  Matrix<T>::Matrix(const size_t _rows,
                    const size_t _cols,
                    const value_type _initVal)
    : Matrix(_rows,_cols,DoNotInitialize) {
    fill(_initVal);
  }

  template<typename T>
  Matrix<T>::Matrix(const size_t _rows,
                    const size_t _cols,
                    const InitializationType)
    : _impl() {
    _create_storage(_rows,_cols);
  }

  /*
   * Construct a matrix rows x cols and initialize all
   * elements with the memory content at the given pointer
   */
  template<typename T>
  Matrix<T>::Matrix(const size_t _rows,
                    const size_t _cols,
                    const const_pointer _initMem)
    : Matrix(_rows,_cols,DoNotInitialize) {
    fill(_initMem);
  }

  template<typename T>
  Matrix<T>::
  Matrix(std::initializer_list< std::initializer_list<value_type> > _lst)
    : Matrix(_lst.size(),
             (_lst.size()>0) ? _lst.begin()->size() : 0,
             DoNotInitialize) {
    fill(_lst);
  }

  template<typename T>
  Matrix<T>::Matrix(const Matrix<T>& _other)
    : Matrix(_other.rows(),_other.cols(),DoNotInitialize) {

    if (std::is_trivial<T>::value) {
      fill(_other.data());
    } else {
      fill(_other);
    }
  }

  template<typename T>
  Matrix<T>::Matrix(Matrix<T>&& _other)
    : _impl() { // move by swap
    this->_impl._swap_data(_other._impl);
  }

  template<typename T>
  Matrix<T>::~Matrix() noexcept {
    _deallocate();
  }

  template<typename T>
  Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    allocate(other._impl._rows, other._impl._cols);

    if (std::is_standard_layout<T>::value) {
      fill(other.data());
    } else {
      fill(other);
    }

    return *this;
  }

  template<typename T>
  Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) {
    if (this->data() != other.data() ) { // alias detection first
      this->_impl._swap_data(other._impl);
      other.clear(); // remove the other's content only if not an alias
    }
    return *this;
  }

  template<typename T>
  bool Matrix<T>::operator==(const Matrix<T>& other) const {
    if (&other==this) return true; // alias detection

    // same size of matrices?
    if ((other.rows() != this->rows()) ||
        (other.cols() != this->cols())) return false;

    //

    // check the content with pointers
    if (std::is_trivially_copyable<T>::value) {
      return (memcmp(this->_impl._data,
                     other._impl._data,
                     this->_impl.tentries()*sizeof(T))==0);
    } else {
      // we have to compare each element
      const const_iterator eit=end();
      for (const_iterator it=begin(),oit=other.begin();
           it!=eit;
           ++it,++oit) {
        if (! (*it == *oit) ) return false;
      }
    }
    return true;
  }

  template<typename T>
  bool Matrix<T>::operator!=(const Matrix<T>& other) const {
    if (&other==this) return false; // alias detection

    return !operator==(other);
  }

  template<typename T>
  void Matrix<T>::swap(Matrix<T>& other) {
    this->_impl._swap_data(other._impl);
  }

  template<typename T>
  void Matrix<T>::allocate(const size_t r,
                           const size_t c) {
    // only reserve iff the desired size is different to the current one
    if ( (r!=rows()) || (c!=cols()) ) {
      _deallocate();
      _create_storage(r,c);
    }
  }

  template<typename T>
  void Matrix<T>::clear() {
    _deallocate();
  }


  template<typename T>
  void Matrix<T>::_create_storage(size_t __rows,size_t __cols) {

    // report an error if this has not previously been initialized
    assert(this->_impl._data == nullptr);

    size_t n = __rows*__cols;

    // Call the allocator to reserve the required memory
    this->_impl._data = new T[n];

    // Initialize the rest of the attributes
    this->_impl._rows = __rows;
    this->_impl._cols = __cols;
  }

  template<typename T>
  void Matrix<T>::_deallocate() {
    if (this->_impl._data) {
      delete[] this->_impl._data;
    }

    this->_impl._data  = nullptr;
    this->_impl._rows  = 0;
    this->_impl._cols  = 0;
  }

  template<typename T>
  void Matrix<T>::fill(const T val) {
    iterator eit = this->end();
    for (iterator ptr = this->begin();ptr!=eit;++ptr) {
      *ptr = val;
    }

    // ACCELERATE ME!
  }

  template<typename T>
  void Matrix<T>::fill(const Matrix<T>& other) {

    const std::size_t r=std::min(other.rows(),this->rows());
    const std::size_t c=std::min(other.cols(),this->cols());

    for (std::size_t j=0u;j<r;++j) {
     for (std::size_t i=0u;i<c;++i) {
	(*this)(j,i)=other(j,i);
      }
    }
  }

  template<typename T>
  void Matrix<T>::fill(const T* mem) {
    std::memcpy(this->_impl._data,mem,sizeof(T)*this->_impl.tentries());
  }

  template<typename T>
  void Matrix<T>::
  fill(const std::initializer_list< std::initializer_list<value_type> >& lst) {

    const size_t r = lst.size();
    const size_t c = (r>0u) ? lst.begin()->size() : 0u;

    assert(r==rows() && "Check number of rows");
    assert(c==cols() && "Check number of cols");

    pointer rowPtr = this->_impl._data;
    for (const auto& r : lst) {
      pointer ptr=rowPtr;

      for (const auto& c : r) {
        *ptr++ = c;
      }
      rowPtr += this->_impl._cols;
    }
  }

  template<typename T>
  std::vector< typename Matrix<T>::value_type >
  Matrix<T>::column(const size_t col) const {

    std::vector< value_type > vct(this->_impl._rows);

    const_pointer ptr = this->_impl._data + col;
    for (auto it=vct.begin(); it!=vct.end(); ++it, ptr+=this->_impl._cols) {
      *it = *ptr;
    }

    return vct;
  }

} // namespace rdf
