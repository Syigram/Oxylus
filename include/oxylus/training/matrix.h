/**
 * Copyright (C) 2017
 * \file:   Matrix.h
 * \author Sygram
 * \author Pablo Alvarado
 */

#ifndef OXYLUS_MATRIX_H
#define OXYLUS_MATRIX_H

#include <oxylus/configuration/Serialization.h>

namespace rdf {

  /**
   * Type to make explicit a desired unitialized matrix
   */
  enum InitializationType {
    DoNotInitialize
  };

  /**
   * Row-major generic matrix class.
   */
  template<typename T>
  class Matrix {
  public:
    /**
     * @name Standard types
     */
    //@{
    /// Contained type
    typedef T value_type;

    /// Pointer to contained type
    typedef T* pointer;

    /// Pointer to constant contained type
    typedef const T* const_pointer;

    /// Iterator
    typedef pointer iterator;

    /// Const interator
    typedef const_pointer const_iterator;
    //@}

  protected:
    /**
     * Data is held in an instance of this.
     *
     * This class has limited functionality but ensures
     * exception safety.
     */
    struct _Matrix_impl {
      /// All matrix data
      pointer _data;

      /// Number of rows
      size_t _rows;

      /// Effective number of columns
      size_t _cols;

      /// Return the total number of entries (i.e. real buffer size)
      inline size_t tentries() const {return _rows*_cols;}

      /**
       * @name Default constructor
       */
      //@
      _Matrix_impl();
      //@}

      void _swap_data(_Matrix_impl& _x) noexcept;
    };

    /// The instance doing all the allocation work
    _Matrix_impl _impl;


  public:

    /**
     * @name Constructors
     */
    //@{
    Matrix();
    explicit Matrix(const size_t _rows,
                    const size_t _cols,
                    const InitializationType _it);
    explicit Matrix(const size_t _rows,
                    const size_t _cols,
                    const value_type _initVal=value_type());
    explicit Matrix(const size_t _rows,
                    const size_t _cols,
                    const const_pointer _initMem);

    Matrix(const Matrix<T>& _other);
    Matrix(Matrix<T>&& _other);
    ~Matrix() noexcept;

    /**
     * Constructs a matrix from a std::initializer_list
     *
     * This allows to construct and initialize a 2x3 matrix in this way:
     *
     * \code
     * anpi::Matrix<int> m={ {1,2,3}, {4,5,6} };
     * \endcode
     */
    Matrix(std::initializer_list< std::initializer_list<value_type> > _lst);

    //@}

    /**
     * Deep copy another matrix of exactly the same type
     */
    Matrix<T>& operator=(const Matrix<T>& other);

    /**
     * Move assignment operator
     */
    Matrix<T>& operator=(Matrix<T>&& other);

    /**
     * Compare two matrices for equality
     *
     * This is slow, as all componentes are elementwise compared
     */
    bool operator==(const Matrix<T>& other) const;

    /**
     * Compare two matrices for equality
     *
     * This is slow, as all componentes are elementwise compared
     */
    bool operator!=(const Matrix<T>& other) const;

    /// Return pointer to a given row
    inline T* operator[](const size_t row) {
      return this->_impl._data + row * this->_impl._cols;
    }

    /// Return read-only pointer to a given row
    const T* operator[](const size_t row) const {
      return this->_impl._data + row * this->_impl._cols;
    }

    /// Return reference to the element at the r row and c column
    T& operator()(const size_t row,const size_t col) {
      return *(this->_impl._data +
               (row*this->_impl._cols + col));
    }

    /// Return const reference to the element at the r row and c column
    const T& operator()(const size_t row,const size_t col) const {
      return *(this->_impl._data +
               (row*this->_impl._cols + col));
    }

    /**
     * Swap the contents of the other matrix with this one
     */
    void swap(Matrix<T>& other);

    /**
     * Allocate memory for the given number of rows and cols
     */
    void allocate(const size_t row,const size_t col);

    /**
     * Reset this matrix to a default constructed empty state
     */
    void clear();

    /**
     * Fill all elements of the matrix with the given value
     */
    void fill(const T val);

    /**
     * Fill all elements of the matrix with the given memory block
     *
     * The user must ensure that the given memory block has enough elements
     * and with the same layout used in this matrix, including the
     * row padding.
     */
    void fill(const T* mem);

    /**
     * Check if the matrix is empty (zero rows or columns)
     */
    inline bool empty() const {
      return ( (this->_impl._rows==0) ||
               (this->_impl._cols==0) );
    }

    /**
     * Number of rows
     */
    inline size_t rows() const {
      return this->_impl._rows;
    }

    /**
     * Number of matrix columns (without any padding)
     */
    inline size_t cols() const {
      return this->_impl._cols;
    }


    /**
     * Total number of entries (rows x cols)
     */
    inline size_t entries() const {
      return this->_impl._rows * this->_impl._cols;
    }

    /**
     * Pointer to data block
     */
    inline T* data() { return this->_impl._data; }

    /**
     * Pointer to data block
     */
    inline const T* data() const { return this->_impl._data; }

    /**
     * Extract one particular column
     *
     * This method has to copy the column, and hence it is relatively slow
     */
    inline std::vector<value_type> column(const size_t col) const;

    /**
     * @name Simple iterator functionality
     */
    //@{
    inline iterator begin();
    inline const_iterator begin() const;
    inline iterator end();
    inline const_iterator end() const;
    //@}

  private:

    // Call the memory deallocation
    void _deallocate();

    /// Use the allocator to create the necessary storage
    void _create_storage(size_t _rows,size_t _cols);

    /**
     * Fill with an initializer list
     */
    void fill(const std::initializer_list<std::initializer_list<value_type> >&);

  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {

      size_t trows(_impl._rows),tcols(_impl._cols);

      // make name-value-pairs (nvp) necessary if XML archives are used
      // other archives will simply ignore the nvp but would work as well.
      /* ar & boost::serialization::make_nvp("rows",trows); */
      /* ar & boost::serialization::make_nvp("cols",tcols); */

      ar & trows;
      ar & tcols;

      if (Archive::is_loading::value) {
        // create a new storage space only if the current size differs
        // from the desired size
        if ( (trows != _impl._rows) ||
             (tcols != _impl._cols) ) {
          clear();
          _create_storage(trows,tcols);
        }
      }

      const size_t entries = _impl.tentries();
      for (size_t i=0;i<entries;++i) {
       /* ar & boost::serialization::make_nvp("item",_impl._data[i]); */
        ar & _impl._data[i];
      }
    }


  }; // class Matrix


} // namespace rdf

// include the template implementations
#include "matrix.tpp"


#endif /* MATRIX_H */
