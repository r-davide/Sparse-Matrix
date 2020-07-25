#ifndef PROJECT_H
#define PROJECT_H

#include <iostream> // std::ostream
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
 * Classe che implementa una matrice sparsa contenente dati generici di tipo T.
 * 
 * @brief Matrice sparsa
 * 
 * @param T tipo del dato
 * @param E funtore di comparazione (==) di due dati di tipo T
 */
template <typename T, typename E>
class sparse_matrix
{
public:
  /**
   * Struttura che implementa un singolo elemento della matrice, 
   * memorizzando quindi la coppia (riga, colonna) e il valore
   * corrispondente a quella cella della matrice.
   * 
   * @brief Elemento della matrice
   */
  struct element
  {
    T value;                ///< Dato inserito nella matrice
    const unsigned int row; ///< Indice di riga dell'elemento
    const unsigned int col; ///< Indice di colonna dell'elemento

    /**
     * Costruttore primario che inizializza un elemento.
     * 
     * @param val valore del dato
     * @param r indice di riga del dato da inserire nella matrice
     * @param c indice di colonna del dato da inserire nella matrice
     */
    element(const T &val, const unsigned int &r, const unsigned int &c)
        : value(val), row(r), col(c) {}

    /**
     * Distruttore.
     */
    ~element() {}

  }; // END struct element

  /**
   * Costruttore primario che inizializza il valore di default della matrice.
   *
   * @param default_value valore di default della matrice
   */
  sparse_matrix(const T &default_value)
      : _elem(nullptr), _size(0), _default(default_value) {}

  /**
   * Costruttore secondario.
   * 
   * Costruisce una matrice di tipo T partendo da una matrice di tipo Q. 
   * 
   * @param other_Q matrice sparsa di tipo Q 
   * 
   * @throw eccezione di allocazione della memoria
   */
  template <typename Q, typename F>
  sparse_matrix(const sparse_matrix<Q, F> &other_Q)
      : _elem(nullptr), _size(0)
  {
    _default = other_Q.get_default();
    typename sparse_matrix<Q, F>::const_iterator it, ite;

    it = other_Q.begin();
    ite = other_Q.end();

    try
    {
      while (it != ite)
      {
        add(it->value, it->row, it->col);
        it++;
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  /**
   * Costruttore di copia.
   * 
   * @param other matrice da copiare 
   * 
   * @throw eccezione di allocazione della memoria
   */
  sparse_matrix(const sparse_matrix &other) : _elem(nullptr), _size(0)
  {
    _default = other._default;
    element **temp = other._elem;

    try
    {
      for (int i = 0; i < other._size; ++i)
      {
        add(temp[i]->value, temp[i]->row, temp[i]->col);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  /**
   * Operatore di assegnamento.
   * 
   * @param other matrice da copiare
   * @return reference a this
   * 
   * @throw eccezione di allocazione della memoria
   */
  sparse_matrix &operator=(const sparse_matrix &other)
  {
    try
    {
      if (this != &other)
      {
        sparse_matrix tmp(other);
        std::swap(_elem, tmp._elem);
        std::swap(_size, tmp._size);
        std::swap(_default, tmp._default);
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
    return *this;
  }

  /**
   * Distruttore.
   */
  ~sparse_matrix()
  {
    clear();
  }

  /**
   * Ritorna il valore di default della matrice.
   * 
   * @return valore di default 
   */
  T get_default() const { return _default; }

  /**
   * Imposta il valore di default.
   * 
   * @param default_value valore di default della matrice 
   */
  void set_default(const T &default_value) { _default = default_value; }

  /**
   * Ritorna il numero di elementi inseriti nella matrice.
   *
   * @return numero di elementi inseriti
   */
  unsigned int get_size() const { return _size; }

  /**
   * Ritorna il numero di righe della matrice.
   *
   * @return numero di righe
   */
  unsigned int get_rows() const
  {
    if (_elem == nullptr)
      return 0;
    else
    {
      return (_elem[_size - 1]->row + 1);
    }
  }

  /**
   * Ritorna il numero di colonne della matrice.
   *
   * @return numero di colonne
   */
  unsigned int get_columns() const
  {
    if (_elem == nullptr)
      return 0;
    else
    {
      int cols = 0;
      for (int i = 0; i < _size; i++)
      {
        if (_elem[i]->col > cols)
          cols = _elem[i]->col;
      }
      return cols + 1;
    }
  }

  /**
   * Aggiunge un elemento nella matrice. 
   * L'ordinamento viene effettuato mediante il metodo sort().
   *
   * @param value valore da inserire
   * @param row indice di riga dove inserire il valore
   * @param col indice di colonna dove inserire il valore
   *
	 * @throw eccezione di allocazione della memoria
	 */
  void add(const T &value, const unsigned int &row, const unsigned int &col)
  {
    try
    {
      if (_elem == nullptr)
      {
        _size = 1;
        _elem = new element *[_size];
        _elem[0] = new element(value, row, col);
      }

      /* 
      controllo nel caso in cui venga richiesto l'inserimento di un valore gia'
      presente in corrispondenza della cella (row, col) in input, in tal caso
      l'inserimento viene ignorato.
      */
      else if (equals_(value, this->operator()(row, col)))
      {
        return;
      }

      /* 
      controllo nel caso in cui vengo richiesto l'inserimento di un nuovo 
      valore in una cella gia' occupata con un valore diverso da quello in input 
      e dal valore di default. In tal caso viene sovrascritto il valore attuale
      con quello nuovo.
      */
      else if (!equals_(_default, this->operator()(row, col)))
      {
        for (int i = 0; i < _size; i++)
        {
          if (_elem[i]->row == row && _elem[i]->col == col)
          {
            _elem[i]->value = value;
            return;
          }
        }
      }

      else
      {
        _size++;
        // int new_size = _size + 1;
        element **temp = new element *[_size];

        for (int i = 0; i < _size - 1; i++)
          temp[i] = _elem[i];

        // deallocazione del puntatore per l'array precedente
        delete[] _elem;
        _elem = nullptr;

        // reimposta il numero di elementi
        // _size = new_size;

        // riassegnamento puntatore al nuovo array
        _elem = new element *[_size];
        for (int i = 0; i < _size - 1; i++)
          _elem[i] = temp[i];

        // accoda il nuovo elemento
        _elem[_size - 1] = new element(value, row, col);

        // deallocazione risorsa inutilizzata;
        delete[] temp;
        temp = nullptr;

        // ordinamento degli elementi in ordine crescente
        sort();
      }
    }
    catch (...)
    {
      clear();
      throw;
    }
  }

  /**
   * Operatore di lettura coordinate.
   *
   * @param row indice di riga
   * @param col indice di colonna
   * 
   * @return valore dell'elemento corrispondente alle coordinate (row, col)
   */
  T &operator()(const unsigned int row, const unsigned int col)
  {
    if (_elem == nullptr)
      return _default;

    for (int i = 0; i < _size; i++)
    {
      if (_elem[i]->row == row && _elem[i]->col == col)
      {
        return _elem[i]->value;
      }
    }
    return _default;
  }

  /**
   * Cancella il contenuto della matrice.
   */
  void clear()
  {
    for (int i = 0; i < _size; i++)
    {
      delete _elem[i];
      _elem[i] = nullptr;
    }
    delete[] _elem;
    _elem = nullptr;
    _size = 0;
  }

  /**
   * Stampa la matrice completa, compresi i valori di default, 
   * utilizzando l'operator().
   */
  void show()
  {
    for (int i = 0; i < this->get_rows(); i++)
    {
      std::cout << std::endl;
      for (int j = 0; j < this->get_columns(); j++)
        std::cout << this->operator()(i, j) << " | ";
    }
    std::cout << std::endl;
  }

  class const_iterator; // forward declaration

  /**
   * Iteratore di tipo forward della matrice.
   * 
   * @brief Iteratore di tipo forward della matrice
   */

  class iterator
  {
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    typedef element *pointer;
    typedef element &reference;

    iterator() : _el(0) {}

    iterator(const iterator &other) : _el(other._el) {}

    iterator &operator=(const iterator &other)
    {
      _el = other._el;
      return *this;
    }

    ~iterator() {}

    // Ritorna il dato riferito dall'iteratore (derefenziamento)
    reference operator*() const { return **_el; }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const { return *_el; }

    // Operatore di iterazione post-incremento
    iterator operator++(int)
    {
      iterator temp(*this);
      _el = ++_el;
      return temp;
    }

    // Operatore di iterazione pre-incremento
    iterator &operator++()
    {
      _el = _el++;
      return *this;
    }

    // Uguaglianza
    bool operator==(const iterator &other) const { return (_el == other._el); }

    // Diversita'
    bool operator!=(const iterator &other) const { return (_el != other._el); }

    friend class const_iterator;

    // Uguaglianza con un const_iterator
    bool operator==(const const_iterator &other) const
    {
      return (_el == other._el);
    }

    // Diversita' con un const_iterator
    bool operator!=(const const_iterator &other) const
    {
      return (_el != other._el);
    }

  private:
    element **_el;

    // Classe container
    friend class sparse_matrix;

    // Costruttore privato di inizializzazione usato dalla classe container
    iterator(element **element) : _el(element) {}

  }; // END class iterator

  /**
   * Ritorna l'iteratore al primo elemento della matrice.
   *
   * @return iteratore al primo elemento della matrice
   */
  iterator begin() { return iterator(_elem); }

  /**
   * Ritorna l'iteratore all'ultimo elemento della matrice.
   *
   * @return iteratore all'ultimo elemento della matrice
   */
  iterator end() { return iterator(_elem + _size); }

  /**
   * Iteratore costante della matrice.
   * 
   * @brief Iteratore costante della matrice
   */

  class const_iterator
  {
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    typedef const element *pointer;
    typedef const element &reference;

    const_iterator() : _el(0) {}

    const_iterator(const const_iterator &other) : _el(other._el) {}

    const_iterator &operator=(const const_iterator &other)
    {
      _el = other._el;
      return *this;
    }

    ~const_iterator() {}

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const { return **_el; }

    // Operatore di iterazione post-incremento
    pointer operator->() const { return *_el; }

    // Operatore di iterazione post-incremento
    const_iterator operator++(int)
    {
      const_iterator temp(*this);
      _el = ++_el;
      return temp;
    }
    // Operatore di iterazione pre-incremento
    const_iterator &operator++()
    {
      _el = _el++;
      return *this;
    }

    // Uguaglianza
    bool operator==(const const_iterator &other) const
    {
      return (_el == other._el);
    }

    // Diversita'
    bool operator!=(const const_iterator &other) const
    {
      return (_el != other._el);
    }

    friend class iterator;

    // Uguaglianza con un iterator
    bool operator==(const iterator &other) const
    {
      return (_el == other._el);
    }

    // Diversita' con un iterator
    bool operator!=(const iterator &other) const
    {
      return (_el != other._el);
    }

    // Costruttore di conversione
    const_iterator(const iterator &other) : _el(other._el) {}

    // Operatore di assegnamento
    const_iterator &operator=(const iterator &other)
    {
      _el = other._el;
      return *this;
    }

  private:
    element **_el;

    // Classe container
    friend class sparse_matrix;

    // Costruttore privato di inizializzazione usato dalla classe container
    const_iterator(const element **element) : _el(element) {}

  }; // END class const_iterator

  /**
   * Ritorna l'iteratore al primo elemento della matrice.
   * 
   * @return iteratore al primo elemento della matrice
	*/
  const_iterator begin() const { return const_iterator(_elem); }

  /**
   * Ritorna l'iteratore all'ultimo elemento della matrice.
   * 
   * @return iteratore all'ultimo elemento della matrice
	*/
  const_iterator end() const { return const_iterator(_elem + _size); }

private:
  element **_elem;    ///< puntatore all'array di elementi della matrice
  T _default;         ///< valore di default della matrice
  unsigned int _size; ///< numero di elementi inseriti nella matrice
  E equals_;          ///< oggetto funtore per l'uguaglianza

  /**
   * Funzione di supporto per ordinare gli elementi della matrice.
   */
  void sort()
  {
    // riordina le righe in ordine crescente
    for (int i = 1; i < _size; i++)
    {
      element *curr = _elem[i];
      int j = i - 1;
      element *prev = _elem[j];

      while (j >= 0 && prev->row > curr->row)
      {
        _elem[j + 1] = _elem[j];
        j--;
        prev = _elem[j];
      }

      _elem[j + 1] = curr;
    }

    // riordina le colonne in ordine crescente aventi medesima riga
    for (int i = 0; i < _size; i++)
    {
      for (int j = i + 1; j < _size; j++)
      {
        element *curr = _elem[j];
        int k = j - 1;
        element *prev = _elem[k];
        while (curr->row == prev->row && k >= 0 && prev->col > curr->col)
        {
          _elem[k + 1] = _elem[k];
          k--;
          prev = _elem[k];
        }
        _elem[k + 1] = curr;
        i = j;
      }
    }
  }
}; // END class sparse_matrix

/**
 * Ridefinizione dell'operatore di stream per la stampa 
 * degli elementi inseriti nella matrice.
 *
 * @param ostream oggetto stream di output
 * @param sm matrice sparsa da stampare
 * 
 * @return reference allo stream di output
 */

template <typename T, typename E>
std::ostream &operator<<(std::ostream &ostream, const sparse_matrix<T, E> &sm)
{
  if (sm.get_size() == 0)
    ostream << sm.get_default() << std::endl;
  else
  {
    typename sparse_matrix<T, E>::const_iterator it, ite;

    it = sm.begin();
    ite = sm.end();

    while (it != ite)
    {
      ostream << "[" << it->row
              << ", " << it->col
              << "] = " << it->value << '\n';
      it++;
    }
  }

  return ostream;
}

/**
 * Funzione generica globale che data una matrice sparsa M e un predicato P
 * ritorna quanti valori in M (compresi i default) soddisfano P.
 * 
 * @param M matrice sparsa di tipo T
 * @param pred predicato da verificare
 * 
 * @return numero di elementi di M che soddisfano P
 */

template <typename T, typename E, typename P>
unsigned int evaluate(const sparse_matrix<T, E> &M, P pred)
{
  int n = 0; // numero di valori in M che soddisfano il predicato pred
  typename sparse_matrix<T, E>::const_iterator it, ite;
  it = M.begin();
  ite = M.end();

  if (M.get_size() > 0)
  {
    if (pred(M.get_default()))
    {
      n += (M.get_rows() * M.get_columns()) - M.get_size();
    }

    while (it != ite)
    {
      if (pred(it->value))
        n++;
      it++;
    }
  }

  return n;
}

#endif // PROJECT_H