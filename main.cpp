#include <cassert>
#include "sparse_matrix.hpp"
#include <string>
#include <vector>

/**
 * Funtore per l'uguaglianza tra tipi int
 *
 * @brief Funtore per l'uguaglianza tra tipi int
 */
struct equals_int
{
  bool operator()(const int &x, const int &y) const
  {
    return x == y;
  }
};

/**
 * Funtore per l'uguaglianza tra tipi long
 *
 * @brief Funtore per l'uguaglianza tra tipi long
 */
struct equals_long
{
  bool operator()(const long &x, const long &y) const
  {
    return x == y;
  }
};

/**
 * Funtore per l'uguaglianza tra stringhe
 *
 * @brief Funtore per l'uguaglianza tra stringhe
 */
struct equals_str
{
  bool operator()(const std::string &str1, const std::string &str2) const
  {
    return str1 == str2;
  }
};

void test_metodi_fondamentali()
{
  std::cout << "***************** TEST FONDAMENTALI ******************"
            << std::endl;

  sparse_matrix<int, equals_int> sm1(0);

  std::cout
      << "Inserimento in sm1 degli elementi 3, 4, 1, 3, 9..."
      << std::endl;

  // inserimento valori
  sm1.add(3, 0, 0);
  sm1.add(9, 4, 2);
  sm1.add(5, 7, 1);
  sm1.add(4, 0, 1);
  sm1.add(1, 3, 3);
  sm1.add(3, 0, 2); // test inserimento doppio dello stesso valore
  sm1.add(3, 0, 2); // viene ignorato la seconda volta

  std::cout << "Numero di elmenti inseriti: " << sm1.get_size() << std::endl;

  std::cout << "Numero di righe: " << sm1.get_rows()
            << "\nNumero di colonne: " << sm1.get_columns()
            << std::endl;

  std::cout << "Stampa completa di sm1 con metodo show(): ";
  sm1.show();

  std::cout << std::endl
            << "Creazione di sm2 tramite copy constructor passando sm1..."
            << std::endl;

  // test copy constructor
  sparse_matrix<int, equals_int> sm2(sm1);

  std::cout
      << "Inserimento in sm2 degli elementi 8, 5..."
      << std::endl;

  // inserimento di ulteriori valori in sm2
  sm2.add(8, 0, 0);
  sm2.add(8, 1, 1);
  sm2.add(5, 10, 6);

  std::cout << "Numero di elementi inseriti: " << sm2.get_size() << std::endl;

  std::cout << "Numero di righe: " << sm2.get_rows()
            << "\nNumero di colonne: " << sm2.get_columns()
            << std::endl;

  std::cout << "Stampa di sm2 con operator<<: "
            << std::endl
            << sm2 << std::endl;

  std::cout << "*************** FINE TEST FONDAMENTALI ***************"
            << std::endl;
}

void test_uso()
{
  std::cout << std::endl
            << "********************** TEST USO **********************"
            << std::endl;

  sparse_matrix<int, equals_int> sm_int(0);

  std::cout << "Inserimento in sm_int degli elementi 1, 2, 6, 3, 9..."
            << std::endl;

  sm_int.add(1, 0, 1);
  sm_int.add(2, 1, 1);
  sm_int.add(6, 3, 2);
  sm_int.add(3, 1, 2);
  sm_int.add(9, 0, 2);

  std::cout << "Numero di elmenti inseriti: " << sm_int.get_size() << std::endl;

  std::cout << "Numero di righe: " << sm_int.get_rows()
            << "\nNumero di colonne: " << sm_int.get_columns()
            << std::endl;

  std::cout << "Stampa di sm_int con operator<<: " << std::endl
            << sm_int << std::endl;

  std::cout << "Creazione di sm_long a partire da sm_int..."
            << std::endl;

  /* 
  test costruttore secondario:
  creazione di una matrice sparsa di tipo long partendo da un'altra di tipo Q
  */
  sparse_matrix<long, equals_long> sm_long(sm_int);

  std::cout << "Numero di elmenti inseriti: " << sm_long.get_size()
            << std::endl;

  std::cout << "Numero di righe: " << sm_long.get_rows()
            << "\nNumero di colonne: " << sm_long.get_columns()
            << std::endl;

  std::cout << "Valore di default copiato da sm_int: "
            << sm_long.get_default() << std::endl;

  std::cout << "Reset del valore di default..." << std::endl;
  sm_long.set_default(10101);

  std::cout << "Nuovo valore di default di sm_long: "
            << sm_long.get_default() << std::endl;

  std::cout
      << "Stampa di sm_long con operator<<: " << std::endl
      << sm_long << std::endl;

  std::cout << "Lettura di alcuni valori di sm_long con operator(): "
            << std::endl
            << "[1, 1] = " << sm_long(1, 1) << std::endl
            << "[6, 9] = " << sm_long(6, 9) << std::endl
            << "[3, 2] = " << sm_long(3, 2) << std::endl;

  assert(sm_long(3, 2) == sm_int(3, 2));
  assert(sm_long(0, 1) == sm_int(0, 1));
  assert(sm_long(1, 1) == sm_int(1, 1));
  assert(sm_long(1, 2) == sm_int(1, 2));

  /* 
  test costruttore secondario:
  eseguendo la riga di codice sottostante, il compilatore genera un errore
  per via dell'incompatibilita' fra tipo long e std::string 
  */
  // sparse_matrix<std::string, equals_str> sm_str(sm_long);

  std::cout << "******************** FINE TEST USO ********************"
            << std::endl;
}

/**
 * Funtore che determina se una stringa str ha un numero di caratteri
 * maggiore di 3.
 *
 * @brief Funtore per valutare la lunghezza di una stringa
 */
struct size_greater_than
{
  bool operator()(const std::string &str) const
  {
    return str.size() > 3;
  }
};

void test_string()
{
  std::cout << std::endl
            << "********************* TEST STRING *********************"
            << std::endl;

  sparse_matrix<std::string, equals_str> sm_string("empty");

  std::cout << "Inserimento in sm_string degli elementi "
            << "e12, e58, e15, e42, e_30..."
            << std::endl;

  sm_string.add("e12", 1, 2);
  sm_string.add("e58", 5, 8);
  sm_string.add("e15", 1, 5);
  sm_string.add("e42", 4, 2);
  sm_string.add("e_30", 3, 0);

  std::cout << "Dimensioni di sm_string: " << sm_string.get_rows()
            << " x " << sm_string.get_columns() << std::endl;
  std::cout << "Numero di elementi inseriti: "
            << sm_string.get_size() << std::endl
            << std::endl;

  sparse_matrix<std::string, equals_str>::const_iterator it, ite;
  it = sm_string.begin();
  ite = sm_string.end();

  std::cout << "Stampa di sm_string con const_iterator:" << std::endl;

  while (it != ite)
  {
    std::cout << "[" << it->row << ", " << it->col
              << "] = " << it->value << std::endl;
    it++;
  }

  std::cout << std::endl;

  size_greater_than gt;

  //test evaluate con funtore size_greater_than
  std::cout << "Test metodo evaluate() con predicato size_greater_than... \n"
            << "Elementi della matrice che soddisfano il predicato: "
            << evaluate(sm_string, gt)
            << std::endl;

  sm_string.clear();
  assert(sm_string.get_size() == 0);

  std::cout << "Dimensioni della matrice dopo clear(): "
            << sm_string.get_rows() << " x "
            << sm_string.get_columns() << std::endl;
  std::cout << "Numero di elementi inseriti dopo clear(): "
            << sm_string.get_size() << std::endl;

  std::cout << "****************** END TEST STRING *******************"
            << std::endl;
}

/**
 * Funtore per l'uguaglianza tra std::vector di tipo stringa
 *
 * @brief Funtore per l'uguaglianza tra std::vector<std::string>
 */
struct equals_vector
{
  bool operator()(const std::vector<std::string> &v1,
                  const std::vector<std::string> &v2)
  {
    return v1 == v2;
  }
};

/**
 * Ridefinizione di operator<< per stampare un std::vector<std::string>
 */
std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &vec)
{
  for (int i = 0; i < vec.size(); ++i)
    os << vec[i] << " ";

  return os;
}

void test_vector()
{
  std::cout << std::endl
            << "******************** TEST VECTOR ********************"
            << std::endl;

  std::cout << "Creazione di un vector di default di tipo string..."
            << std::endl;
  std::vector<std::string> def_vector(1, "<empty>");
  std::cout << "def_vector = " << def_vector[0]
            << ", size() = " << def_vector.size()
            << std::endl;

  std::cout
      << "Creazione di altri due vector string e inserimento nella matrice..."
      << std::endl;

  std::vector<std::string> vec_str1;
  vec_str1.push_back("FOO");
  vec_str1.push_back("BAR");

  std::vector<std::string> vec_str2;
  vec_str2.push_back("PIPPO");
  vec_str2.push_back("PLUTO");

  sparse_matrix<std::vector<std::string>, equals_vector>
      sm_vec(def_vector);

  sm_vec.add(vec_str1, 0, 0);
  sm_vec.add(vec_str2, 2, 1);
  sm_vec.add(vec_str2, 1, 1);
  sm_vec.add(vec_str1, 2, 0);

  std::cout << "Numero di elementi inseriti in sm_vec = "
            << sm_vec.get_size() << "."
            << std::endl;

  std::cout << "Numero righe: " << sm_vec.get_rows()
            << "\nNumero colonne: " << sm_vec.get_columns()
            << std::endl;

  std::cout << "Stampa del contenuto con metodo operator(): "
            << std::endl;

  for (int i = 0; i < sm_vec.get_rows(); i++)
    for (int j = 0; j < sm_vec.get_columns(); j++)
      std::cout << "[" << i << ", " << j << "] = " << sm_vec(i, j) << std::endl;

  def_vector.clear();
  vec_str1.clear();
  vec_str2.clear();

  sm_vec.clear();
  assert(sm_vec.get_size() == 0);

  std::cout << "****************** END TEST VECTOR ******************"
            << std::endl;
}

/**
 * Struct point che implementa un punto 2D.
 *
 * @brief Struct point che implementa un punto 2D.
 */
struct point
{
  int cx; ///< coordinata x del punto
  int cy; ///< coordinata y del punto

  point() : cx(0), cy(0) {}

  point(const int &x, const int &y) : cx(x), cy(y) {}
};

/**
 * Funtore per l'uguaglianza tra due punti.
 * Ritorna true se p.cx == q.cx e p.cy == q.cy
 *
 * @brief Funtore per l'uguaglianza di due punti.
 */
struct equals_point
{
  bool operator()(const point &p, const point &q) const
  {
    return (p.cx == q.cx) && (p.cy == q.cy);
  }
};

/**
 * Ridefinizione di operator<< per stampare una struct point.
 */
std::ostream &operator<<(std::ostream &os, const point &p)
{
  std::cout << "(" << p.cx << "," << p.cy << ")";
  return os;
}

/**
 * Funtore che determina se un punto p e' collocato sull'asse delle ascisse.
 * Ritorna true se la cordinata p.cy e' pari a 0.
 *
 * @brief Funtore per la posizione di un punto rispetto all'asse x.
 */
struct on_x_axis
{
  bool operator()(const point &p) const
  {
    return (p.cy == 0);
  }
};

void test_point()
{
  std::cout << std::endl
            << "********************* TEST POINT *********************"
            << std::endl;

  point origin(0, 0);
  sparse_matrix<point, equals_point> sm_point(origin);

  std::cout << "Inserimento valori (1, 1), (2, 3), (2, 1), (2, 2), (3, 1)..."
            << std::endl;

  sm_point.add(point(1, 1), 0, 0);
  sm_point.add(point(2, 3), 2, 3);
  sm_point.add(point(2, 1), 2, 1);
  sm_point.add(point(2, 2), 2, 2);
  sm_point.add(point(3, 1), 3, 1);

  std::cout << "Dimensioni di sm_point: " << sm_point.get_rows()
            << " x " << sm_point.get_columns() << std::endl;
  std::cout << "Numero di elementi inseriti: "
            << sm_point.get_size() << std::endl;
  std::cout << "Valore di default: " << sm_point.get_default() << std::endl;

  std::cout << std::endl
            << "Stampa completa di sm_point con metodo show(): ";
  sm_point.show();

  std::cout << std::endl
            << "Modifica degli elementi con iterator..." << std::endl;

  sparse_matrix<point, equals_point>::iterator
      it = sm_point.begin(),
      ite = sm_point.end();

  while (it != ite)
  {
    if (it->row == 2)
    {
      it->value = point(2, 0);
    }
    it++;
  }

  // costruzione const_iterator da iterator
  sparse_matrix<point, equals_point>::const_iterator i(it), ie(ite);

  std::cout << "Lettura con const_iterator: "
            << std::endl;

  i = sm_point.begin();

  while (i != ie)
  {
    std::cout << "[" << i->row << ", " << i->col << "] = "
              << i->value << std::endl;
    i++;
  }
  std::cout << std::endl;

  // test evaluate con funtore on_x_axis
  on_x_axis ox;
  std::cout << "Test metodo evaluate() con predicato on_x_axis... \n"
            << "Elementi della matrice che soddisfano il predicato: "
            << evaluate(sm_point, ox) << std::endl;
  std::cout
      << "Gli elementi che soddisfano il predicato: "
      << std::endl;

  if (ox(sm_point.get_default()))
  {
    std::cout << "default = " << sm_point.get_default() << std::endl;
  }

  i = sm_point.begin();
  while (i != ie)
  {
    if (ox(i->value))
      std::cout << "[" << i->row << ", " << i->col << "] = "
                << i->value << std::endl;
    i++;
  }

  std::cout << "****************** END TEST POINT *******************"
            << std::endl;
}

/**
 * Struct voce_rubrica che implementa una voce di una rubrica telefonica
 *
 * @brief Struct voce_rubrica che implementa una voce di rubrica
 */
struct voce_rubrica
{
  std::string nome;    // nome del contatto
  std::string cognome; // cognome del contatto
  std::string ntel;    // numero di telefono del contatto

  voce_rubrica() : nome(""), cognome(""), ntel("") {}

  voce_rubrica(const std::string &n, const std::string &c, const std::string &t)
      : nome(n), cognome(c), ntel(t) {}
};

/**
 * Funtore per l'uguaglianza tra due voci di una rubrica telefonica.
 *
 * @brief Funtore per l'uguaglianza di due voci di rubrica.
 */
struct equals_voce
{
  bool operator()(const voce_rubrica &v1, const voce_rubrica &v2)
  {
    return (v1.ntel == v2.ntel &&
            v1.nome == v2.nome &&
            v1.cognome == v2.cognome);
  }
};

/**
 * Ridefinizione di operator<< per stampare una struct point.
 */
std::ostream &operator<<(std::ostream &os, const voce_rubrica &v)
{
  os << v.nome << " " << v.cognome
     << " " << v.ntel;

  return os;
}

void test_voce()
{
  std::cout << std::endl
            << "********************* TEST VOCE *********************"
            << std::endl;

  voce_rubrica default_v("null", "null", "null");
  voce_rubrica v1("Pippo", "Baudo", "1234");
  voce_rubrica v2("Foo", "Bar", "4242");
  voce_rubrica v3("Donald", "McDonald", "7890");
  voce_rubrica v4("Gerry", "Cala'", "3939");
  voce_rubrica v5("John", "Doe", "5678");

  sparse_matrix<voce_rubrica, equals_voce> sm_voce(default_v);

  std::cout << "Inserimento delle voci_rubrica nella matrice..."
            << std::endl;

  sm_voce.add(v1, 0, 1);
  sm_voce.add(v2, 2, 1);
  sm_voce.add(v3, 1, 3);
  sm_voce.add(v4, 2, 0);
  sm_voce.add(v5, 3, 1);

  std::cout << "Numero di elementi inseriti: "
            << sm_voce.get_size() << std::endl;

  std::cout << "Valore di default della matrice: "
            << sm_voce.get_default() << std::endl;

  std::cout << std::endl
            << "Stampa di sm_voce con operator<<: " << std::endl
            << sm_voce << std::endl;

  std::cout << "Accesso agli elementi con operator(): "
            << std::endl
            << "[2, 0] = " << sm_voce(2, 0) << std::endl
            << "[0, 1] = " << sm_voce(0, 1) << std::endl
            // cella vuota che restituisce il valore di default
            << "[4, 1] = " << sm_voce(4, 1) << std::endl;

  sm_voce.clear();

  std::cout << std::endl
            << "Numero di elementi della matrie dopo clear(): "
            << sm_voce.get_size() << std::endl;
  std::cout << "Dimensione della matrice dopo clear(): "
            << sm_voce.get_rows() << " x "
            << sm_voce.get_columns() << std::endl;

  std::cout << "******************* END TEST VOCE *******************"
            << std::endl;
}

int main(int argc, char const *argv[])
{
  test_metodi_fondamentali();
  test_uso();
  test_string();
  test_vector();
  test_point();
  test_voce();

  return 0;
}
