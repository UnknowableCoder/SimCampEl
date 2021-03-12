#ifndef G24_LIB
#define G24_LIB

/**********************************************
 *                                            *
 *  BIBLIOTECA DE FUNÇÕES MULTI-DIMENSIONAIS  *
 *                                            *
 *     Por: Gonçalo Vaz (87321)               *
 *          Nuno Fernandes (87342)            *
 *     [Antigo Grupo 24 de FC 2017-2018]      *
 * [Antigo Grupo 25 de Programação 2016-2017] *
 *                                            *
 **********************************************/

//Nota dos autores:
//
//Esta biblioteca resulta dos diversos esforços desempenhados
//ao longo da disciplina de Física Computacional de construir
//classes (ou melhor, templates) que facilitassem o tratamento
//dos dados inerentes às diversas simulações computacionais
//exigidas no decurso dos vários projectos que foram pedidos.
//Simultaneamente, os autores pretenderam sempre ir mais além do
//que o que era pedido, tentando criar códigos o mais gerais possível,
//sobretudo em termos do número de dimensões do espaço considerado,
//tendo também em vista a sua aplicabilidade futura, como agora sucede.
//
//De entre os vários elementos que compõem esta biblioteca, será,
//porventura, mais relevante a template coll, que agrega em si
//as funcionalidades de um array n-dimensional de dimensões não
//necessariamente conhecidas no tempo de compilação, possuindo,
//também, a possibilidade de identificar automaticamente todos
//os elementos na fronteira, operar sobre eles, etc. A sua utilidade
//reside, portanto, no tratamento de partições rectangulares n-dimensionais,
//nas quais se poderá associar a cada posição um qualquer dado.
//

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
//Isto tem de vir o mais acima possível
//para que se possa usar a definição de pi.
//(Pelo menos, nalguns compiladores...)

#include <iostream>
#include <fstream>
//Para ostream e afins

#include <vector> //vector
#include <cstdlib> //size_t
#include <cstring> //memcpy
#include <cmath> //Funções matemáticas
#include <limits> //NaN...
#include <random> //Aleatórios...
#include <chrono> //Para as seeds dos aleatórios...
#include <algorithm> //std::max e afins
#include <cctype> //Para std::is_digit.

//Para eventual utilização de funções em CUDA,
//sem impedir a sua inclusão em código normal...
#ifndef CUDA_HOS_DEV
#if defined(__CUDA_ARCH__)
#define CUDA_HOS_DEV __host__ __device__
#else
#define CUDA_HOS_DEV
#endif
#endif

namespace g24_lib
{
  template <class Type, class indexer = size_t, indexer Size = 3> struct fspoint;
  //Forward declaration útil...

  template <class Type, class indexer = size_t> struct point
  //Uma classe para lidar com pontos.
  //Dá para usar qualquer tipo que implemente
  //as operações algébricas usuais e igualdades.
  {
  private:
    Type* val;
    //O array de pontos.
    indexer Size;
    //O comprimento do vector.
  public:
    point()
    //Construtor por defeito...
    {
      Size = 0;
      val = nullptr;
    }
    template <indexer sz> point(const fspoint<Type, indexer, sz> &p)
    {
      Size = sz;
      val = nullptr;
      if (sz)
        {
          val = new Type[sz];
          std::memcpy(val, p.val, (sz) * sizeof(Type));
        }
    }
    point(const indexer n)
    //Construtor inicializado pelo número de elementos.
    {
      Size = n;
      val = nullptr;
      if (n)
        {
          val = new Type[n];
        }
    }
    point (const indexer n, const Type *a)
    //Construtor inicializado por um array de n elementos.
    {
      Size = n;
      val = nullptr;
      if (n)
        {
          val=new Type[n];
          std::memcpy(val, a, (n) * sizeof(Type));
        }
    }
    point(const point<Type, indexer> &p)
    //Construtor por cópia.
    {
      Size = p.Size;
      val = nullptr;
      if (Size)
        {
          val = new Type[p.Size];
          std::memcpy(val, p.val, (p.Size) * sizeof(Type));
        }
    }
    point(const indexer n, const Type &a)
    //Construtor que põe n elementos iguais a a...
    {
      indexer i;
      Size = n;
      val = nullptr;
      if (n)
        {
          val=new Type[n];
          for (i = 0; i < n; ++i)
            {
              val[i] = a;
            }
        }
    }
    point(const std::vector<Type> v)
    //Construtor a partir de um vector da STL.
    {
      Size = v.size();
      val = nullptr;
      if (Size)
        {
          val=new Type[Size];
          std::memcpy(val, v.data(), Size*sizeof(Type));
        }
    }

    const indexer size() const
    {
      return Size;
    }

    point operator+(const point &p) const
    //Soma dois points.
    {
      point temp(Size);
      indexer i, j = (temp.Size > p.Size ? p.Size : temp.Size);
      for (i = 0; i < j; ++i)
        {
          temp.val[i] = val[i] + p.val[i];
        }
      return temp;
    }
    point operator-(const point &p) const
    //Subtrai dois pontos.
    {
      point temp(Size);
      indexer i, j = (temp.Size > p.Size ? p.Size : temp.Size);
      for (i = 0; i < j; ++i)
        {
          temp.val[i] = val[i] - p.val[i];
        }
      return temp;
    }
    point operator+(const Type * const a) const
    //Soma um ponto a um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      point temp(Size);
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = val[i] + a[i];
        }
      return temp;
    }
    point operator-(const Type * const a) const
    //Subtrai um ponto a um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      point temp(Size);
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = val[i] - a[i];
        }
      return temp;
    }

    point operator-() const
    //Inverte o sinal de um ponto.
    {
      point temp(Size);
      indexer i;
      for (i = 0; i < temp.Size; ++i)
        {
          temp.val[i] = -val[i];
        }
      return temp;
    }
    point operator*(const Type &a) const
    //Multiplica um ponto por um escalar.
    {
      point temp(Size);
      indexer i;
      for (i = 0; i < temp.Size; ++i)
        {
          temp.val[i] = val[i]*a;
        }
      return temp;
    }
    point operator/(const Type &a) const
    //Divide um ponto por um escalar.
    {
      point temp(Size);
      indexer i;
      for (i = 0; i < temp.Size; ++i)
        {
          temp.val[i] = val[i]/a;
        }
      return temp;
    }
    Type& operator[](const indexer i)
    //Acede aos elementos de um vector.
    {
      if (i >= Size)
        {
          std::cerr << "\n Trying to access undefined element " << i << " of vector:\n" << (*this) << std::endl;
          return val[Size - 1];
          //No caso de o vector ser nulo,
          //haverá falha de segmentação,
          //mas poder-se-á considerar culpa
          //do utilizador...
        }
      return val[i];
    }
    Type operator[](const indexer i) const
    //Acede aos elementos de um vector constante
    //(não permitindo a sua alteração).
    {
      if (i >= Size)
        {
          std::cerr << "\n Trying to access undefined element " << i << " of vector:\n" << (*this) << std::endl;
          return val[Size - 1];
          //No caso de o vector ser nulo,
          //haverá falha de segmentação,
          //mas poder-se-á considerar culpa
          //do utilizador...
        }
      return val[i];
    }
    bool operator== (const point &p) const
    //Compara vectores.
    {
      indexer i;
      if (Size != p.Size)
        {
          return false;
        }
      for (i = 0; i < Size; ++i)
        {
          if (val[i] != p.val[i])
            {
              return false;
            }
        }
      return true;
    }
    bool operator!= (const point &p) const
    //Compara diferenças de vectores.
    {
      return !(p == *this);
    }
    point& operator=(const point &p)
    //Põe um vector igual a outro.
    {
      if(&p == this)
        {
          return (*this);
        }
      if(Size != p.Size)
        {
          Size = p.Size;
          if (val)
            {
              delete[] val;
              val = nullptr;
            }
          if(Size)
            {
              val = new Type[Size];
            }
        }
      if(Size)
        {
          std::memcpy(val, p.val, (Size) * sizeof(Type));
        }
      return *this;
    }
    point& operator+=(const point &p)
    //Põe um vector igual à sua soma com outro.
    {
      indexer i, j = (Size > p.Size ? p.Size : Size);
      for (i = 0; i < j; ++i)
        {
          val[i] += p.val[i];
        }
      return *this;
    }
    point& operator-=(const point &p)
    //Põe um vector igual à sua subtracção com outro.
    {
      indexer i, j = (Size > p.Size ? p.Size : Size);
      for (i = 0; i < j; ++i)
        {
          val[i] -= p.val[i];
        }
      return *this;
    }
    point& operator+=(const Type * const a)
    //Põe um vector igual à sua soma com um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          val[i] += a[i];
        }
      return *this;
    }
    point& operator-=(const Type * const a)
    //Põe um ponto igual à sua subtracção com a um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          val[i] -= a[i];
        }
      return *this;
    }

    point& operator*=(const Type &a)
    //Põe um ponto igual à sua multiplicação por escalar
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          val[i] *= a[i];
        }
      return *this;
    }

    point& operator/=(const Type &a)
    //Põe um ponto igual à sua divisão por escalar
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          val[i] /= a[i];
        }
      return *this;
    }

    point& operator<< (const Type &x)
    //Adiciona um valor ao point.
    {
      Type *temp = new Type[Size + 1];
      std::memcpy(temp, val, (Size) * sizeof(Type));
      temp[Size] = x;
      Size = Size + 1;
      delete[] val;
      val = temp;
      return *this;
    }

    point& operator<< (const point &p)
    //Concatena points.
    {
      Type *temp = new Type[Size + p.Size];
      std::memcpy(temp, val, (Size) * sizeof(Type));
      std::memcpy(temp + Size, p.val, (p.Size) * sizeof(Type));
      Size = Size + p.Size;
      delete[] val;
      val = temp;
      return *this;
    }

    point& operator>> (const indexer n)
    //Retira n valores do point.
    {
      if (n >= Size)
        {
          if (n > Size)
            {
              std::cerr << "\n Trying to remove more elements (" << n - Size
                        << ") than the vector has:\n" << (*this) << std::endl;
            }
          delete[] val;
          val = nullptr;
          Size = 0;
          return *this;
        }
      Type *temp = new Type[Size - n];
      std::memcpy(temp, val, (Size-n) * sizeof(Type));
      delete[] val;
      val = temp;
      return *this;
    }

    void raw_print(std::ostream &s, bool print_Size = false) const
    //Uma função para imprimir coisas, sem o refinamento do operador <<...
    //O argumento opcional print_Size permite imprimir o número de elementos,
    //para facilitar eventuais leituras posteriores...
    {
      indexer i;
      if (print_Size)
        {
          s << " " << Size;
        }
      if (Size)
        {
          s << " ";
          for (i = 0; i < Size; ++i)
            {
              s << val[i] << " ";
            }
        }
    }


    Type sum_all() const
    //Soma todos os elementos do point.
    {
      indexer a;
      if(Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              ret +=val[a];
            }
          return ret;
        }
      return (Type) 0;
    }
    Type max() const
    //Retorna o elemento de valor máximo.
    {
      indexer a;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret < val[a])
                {
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }
    Type max(indexer *comp) const
    //Retorna o elemento de valor máximo e coloca
    //no endereço de comp o índice do elemento que o possui.
    {
      indexer a;
      (*comp) = 0;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret < val[a])
                {
                  (*comp) = a;
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }

    Type min() const
    //Retorna o elemento de valor mínimo.
    {
      indexer a;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret > val[a])
                {
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }
    Type min(indexer *comp) const
    //Retorna o elemento de valor mínimo e coloca
    //no endereço de comp o índice do elemento que o possui.
    {
      indexer a;
      (*comp) = 0;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret > val[a])
                {
                  (*comp) = a;
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }

    Type square_norm2() const
    //Retorna o quadrado da norma do vector na norma-2,
    //isto é, o resultado de v interno com v...
    {
      indexer i;
      Type ret = (Type) 0;
      for (i = 0; i < Size; ++i)
        {
          ret += val[i]*val[i];
        }
      return ret;
    }

    point<Type, indexer> normalize(const indexer k = std::numeric_limits<indexer>::max(), const double norm = 1.0, const double p = 2.0) const
    //Normaliza o vector na norma p,
    //mantendo a k-ésima componente positiva
    //(no caso em que k indique uma componente
    //inexistente do vector, não é feita qualquer
    //alteração do seu sentido).
    {
      point<Type, indexer> temp(*this);
      double f = ::fabs(*this, p)/norm;
      if (std::isnan(f))
        {
          return temp;
        }
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = temp.val[i]/ (Type) f;
        }
      return temp * (Type) (k >= Size ?  1.0 : (temp.val[k] < 0 ? -1.0 : 1.0));
    }

    void assure_Size(const indexer desired)
    //Garante que o vector tem um dado tamanho.
    {
      if (Size == desired)
        {
          return;
        }
      if (Size > desired)
        {
          (*this) >> (Size - desired);
          return;
        }
      Type *temp = new Type[desired];
      std::memcpy(temp, val, (Size) * sizeof(Type));
      delete[] val;
      val = temp;
      Size = desired;
    }

    ~point()
    {
      delete[] val;
    }

    void set_all(const Type& a)
    //Põe todos os elementos do vector com um dado valor.
    {
      indexer i;
      for(i = 0; i < Size; ++i)
        {
          val[i] = a;
        }
    }

    void inc_with_wrap(point<Type, indexer> &limits, const indexer where = 0, bool reverse = false, bool brk = false)
    //Incrementa os valores do point a começar em where,
    //colocando-os a zero se ultrapassarem o valor correspondente
    //no ponto dado em limits. Se reverse for verdadeiro,
    //incrementa desde where até aos valores anteriores.
    //Se brk for verdadeiro, assim que se obtém um valor
    //que esteja dentro dos considerados, pára-se a verificação
    //(para os casos em que se saiba que o point, a menos do
    //incremento, está todo dentro dos limites).
    {
      indexer i = std::max(where, Size-1);
      if (where >= Size)
        {
          std::cerr << "\n Trying to sum with wrap beginning in non-existent element ( "
                    << where << " ) of point " << (*this) << " !"  <<std::endl;
        }
      val[i] = val[i] + (Type) 1;
      if (reverse)
        {
          for(i = i - 1; i >= 0; --i)
            {
              if (val[i+1] >= limits[i+1])
                {
                  val[i] = val [i] + (Type) 1;
                  val[i+1] = (Type) 0;
                }
              else if(brk)
                {
                  break;
                }
              if (i == 0)
                {
                  break;
                }
            }
        }
      else
        {
          for(i = i + 1; i < Size; ++i)
            {
              if (val[i-1] >= limits[i-1])
                {
                  val[i] = val[i] + 1;
                  val[i-1] = 0;
                }
              else if(brk)
                {
                  break;
                }
            }
        }
    }

    friend std::ostream& operator<< (std::ostream &s, const point<Type, indexer> &p)
    //Imprime um point.
    {
      indexer i, j = p.Size - 1;
      if (p.Size)
        {
          s << "(";
          for (i = 0; i < j; ++i)
            {
              s << p.val[i] << ", ";
            }
          s << p.val[p.Size-1] << ")";
        }
      return s;
    }

    friend Type dotp(const point<Type, indexer> &u, const point<Type, indexer> &v)
    //Faz o produto escalar de dois vectores.
    {
      if(u.Size != v.Size)
        {
          std::cerr << "Incompatible vector sizes for dot product!" << std::endl;
          return std::numeric_limits<double>::quiet_NaN();
        }
      indexer i;
      Type ret;
      ret = u[0]*v[0];
      for (i = 1; i < u.Size; ++i)
        {
          ret += u[i]*v[i];
        }
      return ret;
    }

    friend point<Type, indexer> crossp(const point<Type, indexer> &u, const point<Type, indexer> &v)
    {
      if (u.Size != 3 || v.Size != 3)
        {
          std::cerr << "Incompatible vector sizes for cross product!" << std::endl;
          return u;
        }
      point <Type, indexer> ret(3);
      ret.val[0] = u.val[1] * v.val[2] - u.val[2] * v.val[1];
      ret.val[1] = u.val[2] * v.val[0] - u.val[0] * v.val[2];
      ret.val[2] = u.val[0] * v.val[1] - u.val[1] * v.val[0];
      return ret;
    }



    point<Type, indexer> project(const point<Type, indexer> &p, const bool normal = false) const
    //Faz a projecção do vector segundo p.
    //O argumento opcional serve para vectores que se
    //saiba estarem normalizados, para poupar cálculos...
    {
      if (normal)
        {
          return p * (dotp((*this), p));
        }
      return p * (dotp((*this), p)/dotp(p, p));
    }


    point<Type, indexer> ortog(const point<Type, indexer> &p, const bool normal = false) const
    //Dá a componente do vector que é ortogonal a p.
    //O argumento opcional serve para vectores que se
    //saiba estarem normalizados, para poupar cálculos...
    {
      return ((*this) - project(p, normal));
    }

    friend point<Type, indexer> project(const point<Type, indexer> &a, const point<Type, indexer> &b, const bool normal = false)
    //No sentido em que pode ser mais claro fazer-se
    //project(vector1, vector2) do que vector1.project(vector2),
    //dá-se ao utilizador a possibilidade de escrever de ambas as maneiras...
    {
      return a.project(b, normal);
    }

    friend point<Type, indexer> ortog(const point<Type, indexer> &a, const point<Type, indexer> &b, const bool normal = false)
    //No sentido em que pode ser mais claro fazer-se
    //ortog(vector1, vector2) do que vector1.ortog(vector2),
    //dá-se ao utilizador a possibilidade de escrever de ambas as maneiras...
    {
      return a.ortog(b, normal);
    }

    Type angle(const point<Type, indexer> &p) const
    //Dá o ângulo entre os vectores.
    {
      if(Size != p.Size)
        {
          std::cerr << "Incompatible vector sizes for determining the angle!" << std::endl;
          return std::numeric_limits<double>::quiet_NaN();
        }
      Type ret = 0, l1 = 0, l2 = 0;
      indexer i;
      for(i = 0; i < Size; ++i)
        {
          ret += val[i]*p.val[i];
          l1 += val[i]*val[i];
          l2 += p.val[i]*p.val[i];
        }
      return std::acos(ret/sqrt(l1*l2));
    }

    friend Type angle(const point<Type, indexer> &v, const point <Type, indexer> &u)
    //Mais uma vez, para conforto e facilidade de uso.
    {
      return v.angle(u);
    }

    Type cosangle(const point<Type, indexer> &p) const
    //Dá o co-seno do ângulo entre os dois vectores.
    {
      if(Size != p.Size)
        {
          std::cerr << "Incompatible vector sizes for determining the angle!" << std::endl;
          return std::numeric_limits<double>::quiet_NaN();
        }
      Type ret = 0, l1 = 0, l2 = 0;
      indexer i;
      for(i = 0; i < Size; ++i)
        {
          ret += val[i]*p.val[i];
          l1 += val[i]*val[i];
          l2 += p.val[i]*p.val[i];
        }
      return (ret/sqrt(l1*l2));
    }

    friend Type cosangle(const point<Type, indexer> &v, const point <Type, indexer> &u)
    //Mais uma vez, para conforto e facilidade de uso.
    {
      return v.cosangle(u);
    }

    friend point<Type, indexer> operator*(const Type &a, const point<Type, indexer> &p)
    //Uma função que permite multiplicar um escalar por um vector,
    //pelo simples motivo de que, em certos casos, pode ser útil
    //escrever "escalar" * "vector" em vez do contrário.
    {
      return p*a;
    }

    Type * get_access_to_array()
    //Apenas e só para a implementação em CUDA..
    {
      return val;
    }

    void raw_read(std::istream &s)
    //Lê um point a partir de uma stream,
    //na forma "número de elementos" elemento_1 elemento_2...
    {
      indexer a, i;
      s >> a;
      if (s.fail())
        {
          std::cerr << "Unable to read size of point!" << std::endl;
        }
      a = 0;
      point<Type, indexer> ret(a);
      for (i = 0; i < a; ++i)
        {
          s >> ret.val[i];
        }
      (*this) = ret;
    }

    void raw_read(std::istream &s, const indexer num)
    //Lê um point a partir de uma stream,
    //com um número conhecido de elementos (num).
    {
      indexer i;
      point<Type, indexer> ret(num);
      for (i = 0; i < num; ++i)
        {
          s >> ret.val[i];
        }
      (*this) = ret;
    }

  };

  template <class Type, class indexer = size_t> point<Type, indexer> raw_read_point (std::istream &s)
  //Cria um point lido a partir de uma stream,
  //na forma "número de elementos" elemento_1 elemento_2...
  {
    indexer a, i;
    s >> a;
    if (s.fail())
      {
        std::cerr << "Unable to read Size of point!" << std::endl;
      }
    a = 0;
    point<Type, indexer> ret(a);
    for (i = 0; i < a; ++i)
      {
        s >> ret[i];
      }
    return ret;
  }

  template <class Type, class indexer = size_t> point<Type, indexer> raw_read_point (std::istream &s, const indexer num)
  //Cria um point lido a partir de uma stream,
  //com um número conhecido de elementos (num).
  {
    indexer i;
    point<Type, indexer> ret(num);
    for (i = 0; i < num; ++i)
      {
        s >> ret[i];
      }
    return ret;
  }
}
template <class Type, class indexer = size_t> double fabs(const g24_lib::point <Type, indexer> &p, const double num = 2.0)
//O "módulo" de um vector é tomado
//como a sua norma-num, sendo usada,
//por defeito, a norma-2.
{
  indexer i;
  double ret = 0.0;
  if(num < 1.0)
    {
      std::cerr << "\nInvalid norm!" << std::endl;
      return std::numeric_limits<double>::quiet_NaN();
    }
  if (std::isinf(num))
    {
      for (i = 0; i < p.size(); ++i)
        {
          ret = std::max(ret,fabs(p[i]));
        }
      return ret;
    }
  for (i = 0; i < p.size(); ++i)
    {
      ret += pow(fabs(p[i]), num);
    }
  return pow(ret, 1.0/num);
}

template <class Type, class indexer = size_t> float fabsf(const g24_lib::point <Type, indexer> &p, const float num = 2.0)
//O "módulo" de um vector é tomado
//como a sua norma-num, sendo usada,
//por defeito, a norma-2.
{
  indexer i;
  float ret = 0.0;
  if(num < 1.0f)
    {
      std::cerr << "\nInvalid norm!" << std::endl;
      return std::numeric_limits<float>::quiet_NaN();
    }
  if (std::isinf(num))
    {
      for (i = 0; i < p.size(); ++i)
        {
          ret = std::max(ret,fabs(p[i]));
        }
      return ret;
    }
  for (i = 0; i < p.size(); ++i)
    {
      ret += powf(fabsf(p[i]), num);
    }
  return powf(ret, 1.0f/num);
}

template <class Type, class indexer = size_t, indexer Size = 3> double fabs(const g24_lib::fspoint <Type, indexer, Size> &p, const double num = 2.0)
//O "módulo" de um vector é tomado
//como a sua norma-num, sendo usada,
//por defeito, a norma-2.
{
  indexer i;
  double ret = 0.0;
  if(num < 1.0)
    {
      std::cerr << "\nInvalid norm!" << std::endl;
      return std::numeric_limits<double>::quiet_NaN();
    }
  if (std::isinf(num))
    {
      for (i = 0; i < Size; ++i)
        {
          ret = std::max(ret,::fabs(p[i]));
        }
      return ret;
    }
  for (i = 0; i < Size; ++i)
    {
      ret += pow(fabs(p[i]), num);
    }
  return pow(ret, 1.0/num);
}

template <class Type, class indexer = size_t, indexer Size = 3> float fabsf(const g24_lib::fspoint <Type, indexer, Size> &p, const float num = 2.0)
//O "módulo" de um vector é tomado
//como a sua norma-num, sendo usada,
//por defeito, a norma-2.
{
  indexer i;
  float ret = 0.0;
  if(num < 1.0f)
    {
      std::cerr << "\nInvalid norm!" << std::endl;
      return std::numeric_limits<float>::quiet_NaN();
    }
  if (std::isinf(num))
    {
      for (i = 0; i < Size; ++i)
        {
          ret = std::max(ret,fabs(p[i]));
        }
      return ret;
    }
  for (i = 0; i < Size; ++i)
    {
      ret += powf(fabsf(p[i]), num);
    }
  return powf(ret, 1.0f/num);
}

namespace g24_lib
{

  template <class Type, class indexer, indexer Size> struct fspoint
  //Uma classe para lidar com pontos.
  //Um point de tamanho fixo em compile time...
  {
  private:
    Type val[Size];
    //O array de pontos.
  public:

    template <class A, class b, b> friend struct fspoint;
    //Isto é uma forma um pouco enviesada de conseguir o que se pretende,
    //que é permitir os fspoint<Type, indexer, Size> sejam friends
    //para vários Size (mas mesmos Type e indexer).
    //A perda de encapsulamento é, ainda assim, menor
    //do que a alternativa óbvia, que é tornar val público...

    friend struct point<Type, indexer>;

    fspoint()
    //Construtor por defeito...
    {
    }
    fspoint (const Type *a)
    //Construtor inicializado por um array, que se assume suficientemente grande.
    {
      std::memcpy(val, a, (Size) * sizeof(Type));
    }
    fspoint(const fspoint<Type, indexer, Size> &p)
    //Construtor por cópia.
    {
      std::memcpy(val, p.val, (Size) * sizeof(Type));
    }
    fspoint(const Type &a)
    //Construtor que põe os elementos iguais a a...
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          val[i] = a;
        }
    }
    fspoint(const std::vector<Type> v)
    //Construtor a partir de um vector da STL.
    {
      std::memcpy(val, v.data(), Size*sizeof(Type));
    }

    // /*
    // const indexer size() const
    // {
    //   return Size;
    // } */

    template<indexer Size2> fspoint<Type, indexer, Size> operator+(const fspoint<Type, indexer, Size2> &p) const
    //Soma dois points.
    {
      fspoint temp;
      indexer i, j = (Size > Size2 ? Size2 : Size);
      for (i = 0; i < j; ++i)
        {
          temp.val[i] = val[i] + p.val[i];
        }
      return temp;
    }
    template<indexer Size2> fspoint<Type, indexer, Size> operator-(const fspoint<Type, indexer, Size2> &p) const
    //Subtrai dois pontos.
    {
      fspoint temp;
      indexer i, j = (Size > Size2 ? Size2 : Size);
      for (i = 0; i < j; ++i)
        {
          temp.val[i] = val[i] - p.val[i];
        }
      return temp;
    }
    fspoint operator+(const Type * const a) const
    //Soma um ponto a um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      fspoint temp;
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = val[i] + a[i];
        }
      return temp;
    }
    fspoint operator-(const Type * const a) const
    //Subtrai um ponto a um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      fspoint temp;
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = val[i] - a[i];
        }
      return temp;
    }

    fspoint operator-() const
    //Inverte o sinal de um ponto.
    {
      fspoint temp;
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = -val[i];
        }
      return temp;
    }
    fspoint operator*(const Type &a) const
    //Multiplica um ponto por um escalar.
    {
      fspoint temp;
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = val[i]*a;
        }
      return temp;
    }
    fspoint operator/(const Type &a) const
    //Divide um ponto por um escalar.
    {
      fspoint temp;
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = val[i]/a;
        }
      return temp;
    }
    Type& operator[](const indexer i)
    //Acede aos elementos de um vector.
    {
      if (i >= Size)
        {
          std::cerr << "\n Trying to access undefined element " << i << " of vector:\n" << (*this) << std::endl;
          return val[Size - 1];
          //No caso de o vector ser nulo,
          //haverá falha de segmentação,
          //mas poder-se-á considerar culpa
          //do utilizador...
        }
      return val[i];
    }
    Type operator[](const indexer i) const
    //Acede aos elementos de um vector constante
    //(não permitindo a sua alteração).
    {
      if (i >= Size)
        {
          std::cerr << "\n Trying to access undefined element " << i << " of vector:\n" << (*this) << std::endl;
          return val[Size - 1];
          //No caso de o vector ser nulo,
          //haverá falha de segmentação,
          //mas poder-se-á considerar culpa
          //do utilizador...
        }
      return val[i];
    }
    bool operator== (const fspoint &p) const
    //Compara vectores.
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          if (val[i] != p.val[i])
            {
              return false;
            }
        }
      return true;
    }
    bool operator!= (const fspoint &p) const
    //Compara diferenças de vectores.
    {
      return !(p == *this);
    }
    fspoint& operator=(const fspoint &p)
    //Põe um vector igual a outro.
    {
      if(&p == this)
        {
          return (*this);
        }
      std::memcpy(val, p.val, (Size) * sizeof(Type));
      return *this;
    }
    template <indexer Size2> fspoint& operator+=(const fspoint<Type, indexer, Size2> &p)
    //Põe um vector igual à sua soma com outro.
    {
      indexer i, j = (Size > Size2 ? Size2 : Size);
      for (i = 0; i < j; ++i)
        {
          val[i] += p.val[i];
        }
      return *this;
    }
    template <indexer Size2> fspoint& operator-=(const fspoint<Type, indexer, Size2> &p)
    //Põe um vector igual à sua subtracção com outro.
    {
      indexer i, j = (Size > Size2 ? Size2 : Size);
      for (i = 0; i < j; ++i)
        {
          val[i] -= p.val[i];
        }
      return *this;
    }
    fspoint& operator+=(const Type * const a)
    //Põe um vector igual à sua soma com um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          val[i] += a[i];
        }
      return *this;
    }
    fspoint& operator-=(const Type * const a)
    //Põe um ponto igual à sua subtracção com a um array de tipos
    //(que se assume ser suficientemente comprido);
    {
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          val[i] -= a[i];
        }
      return *this;
    }

    fspoint<Type, indexer, Size+1> operator<< (const Type &x)
    //Adiciona um valor ao point.
    //Visto que se muda de tipo,
    //o retorno não é por referência,
    //sendo, a menos de optimizaões,
    //menos eficiente fazer p << 1 << 2...
    {
      Type temp[Size + 1];
      std::memcpy(temp, val, (Size) * sizeof(Type));
      temp[Size] = x;
      return fspoint<Type, indexer, Size+1>(temp);
    }

    template <indexer Size2> fspoint<Type, indexer, Size + Size2> operator<< (const fspoint<Type, indexer, Size2> &p)
    //Concatena points.
    //Visto que se muda de tipo,
    //o retorno não é por referência,
    //sendo, a menos de optimizaões,
    //menos eficiente fazer p << q << r...
    {
      Type temp[Size + Size2];
      std::memcpy(temp, val, (Size) * sizeof(Type));
      std::memcpy(temp + Size, p.val, Size2 * sizeof(Type));
      return fspoint<Type, indexer, Size + Size2>(temp);
    }

    void raw_print(std::ostream &s, bool print_Size = false) const
    //Uma função para imprimir coisas, sem o refinamento do operador <<...
    //O argumento opcional print_Size permite imprimir o número de elementos,
    //para facilitar eventuais leituras posteriores...
    {
      indexer i;
      if (print_Size)
        {
          s << " " << Size;
        }
      if (Size)
        {
          s << " ";
          for (i = 0; i < Size; ++i)
            {
              s << val[i] << " ";
            }
        }
    }


    Type sum_all() const
    //Soma todos os elementos do point.
    {
      indexer a;
      if(Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              ret +=val[a];
            }
          return ret;
        }
      return (Type) 0;
    }
    Type max() const
    //Retorna o elemento de valor máximo.
    {
      indexer a;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret < val[a])
                {
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }
    Type max(indexer *comp) const
    //Retorna o elemento de valor máximo e coloca
    //no endereço de comp o índice do elemento que o possui.
    {
      indexer a;
      (*comp) = 0;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret < val[a])
                {
                  (*comp) = a;
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }

    Type min() const
    //Retorna o elemento de valor mínimo.
    {
      indexer a;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret > val[a])
                {
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }
    Type min(indexer *comp) const
    //Retorna o elemento de valor mínimo e coloca
    //no endereço de comp o índice do elemento que o possui.
    {
      indexer a;
      (*comp) = 0;
      if (Size)
        {
          Type ret = val[0];
          for (a = 1; a < Size; ++a)
            {
              if (ret > val[a])
                {
                  (*comp) = a;
                  ret = val[a];
                }
            }
          return ret;
        }
      return (Type) 0;
    }

    Type square_norm2() const
    //Retorna o quadrado da norma do vector na norma-2,
    //isto é, o resultado de v interno com v...
    {
      indexer i;
      Type ret = (Type) 0;
      for (i = 0; i < Size; ++i)
        {
          ret += val[i]*val[i];
        }
      return ret;
    }

    fspoint<Type, indexer, Size> normalize(const indexer k = std::numeric_limits<indexer>::max(), const double norm = 1.0, const double p = 2.0) const
    //Normaliza o vector na norma p,
    //mantendo a k-ésima componente positiva
    //(no caso em que k indique uma componente
    //inexistente do vector, não é feita qualquer
    //alteração do seu sentido).
    {
      fspoint<Type, indexer, Size> temp(*this);
      double f = ::fabs<Type>((*this), p)/norm;
      if (std::isnan(f))
        {
          return temp;
        }
      indexer i;
      for (i = 0; i < Size; ++i)
        {
          temp.val[i] = temp.val[i]/ (Type) f;
        }
      return temp * (Type) (k >= Size ?  1.0 : (temp.val[k] < 0 ? -1.0 : 1.0));
    }

    void set_all(const Type& a)
    //Põe todos os elementos do vector com um dado valor.
    {
      indexer i;
      for(i = 0; i < Size; ++i)
        {
          val[i] = a;
        }
    }

    void inc_with_wrap(fspoint<Type, indexer, Size> &limits, const indexer where = 0, bool reverse = false, bool brk = false)
    //Incrementa os valores do point a começar em where,
    //colocando-os a zero se ultrapassarem o valor correspondente
    //no ponto dado em limits. Se reverse for verdadeiro,
    //incrementa desde where até aos valores anteriores.
    //Se brk for verdadeiro, assim que se obtém um valor
    //que esteja dentro dos considerados, pára-se a verificação
    //(para os casos em que se saiba que o point, a menos do
    //incremento, está todo dentro dos limites).
    {
      indexer i = std::max(where, Size-1);
      if (where >= Size)
        {
          std::cerr << "\n Trying to sum with wrap beginning in non-existent element ( "
                    << where << " ) of fspoint " << (*this) << " !"  <<std::endl;
        }
      val[i] = val[i] + (Type) 1;
      if (reverse)
        {
          for(i = i - 1; i >= 0; --i)
            {
              if (val[i+1] >= limits[i+1])
                {
                  val[i] = val [i] + (Type) 1;
                  val[i+1] = (Type) 0;
                }
              else if(brk)
                {
                  break;
                }
              if (i == 0)
                {
                  break;
                }
            }
        }
      else
        {
          for(i = i + 1; i < Size; ++i)
            {
              if (val[i-1] >= limits[i-1])
                {
                  val[i] = val[i] + 1;
                  val[i-1] = 0;
                }
              else if(brk)
                {
                  break;
                }
            }
        }
    }

    friend std::ostream& operator<< (std::ostream &s, const fspoint<Type, indexer, Size> &p)
    //Imprime um point.
    {
      indexer i, j = Size - 1;
      if (Size)
        {
          s << "(";
          for (i = 0; i < j; ++i)
            {
              s << p.val[i] << ", ";
            }
          s << p.val[Size-1] << ")";
        }
      return s;
    }

    friend Type dotp(const fspoint<Type, indexer, Size> &u, const fspoint<Type, indexer, Size> &v)
    //Faz o produto escalar de dois vectores.
    {
      indexer i;
      Type ret;
      if (Size)
        {
          ret = u[0]*v[0];
          for (i = 1; i < Size; ++i)
            {
              ret += u[i]*v[i];
            }
          return ret;
        }
      return (Type) 0;
    }

    friend fspoint<Type, indexer, Size> crossp(const fspoint<Type, indexer, Size> &u, const fspoint<Type, indexer, Size> &v)
    {
      if (Size != 3)
        {
          std::cerr << "Incompatible vector sizes for cross product!" << std::endl;
          return u;
        }
      fspoint <Type, indexer, Size> ret;
      ret.val[0] = u.val[1] * v.val[2] - u.val[2] * v.val[1];
      ret.val[1] = u.val[2] * v.val[0] - u.val[0] * v.val[2];
      ret.val[2] = u.val[0] * v.val[1] - u.val[1] * v.val[0];
      return ret;
    }



    fspoint<Type, indexer, Size> project(const fspoint<Type, indexer, Size> &p, const bool normal = false) const
    //Faz a projecção do vector segundo p.
    //O argumento opcional serve para vectores que se
    //saiba estarem normalizados, para poupar cálculos...
    {
      if (normal)
        {
          return p * (dotp((*this), p));
        }
      return p * (dotp((*this), p)/dotp(p, p));
    }


    fspoint<Type, indexer, Size> ortog(const fspoint<Type, indexer, Size> &p, const bool normal = false) const
    //Dá a componente do vector que é ortogonal a p.
    //O argumento opcional serve para vectores que se
    //saiba estarem normalizados, para poupar cálculos...
    {
      return ((*this) - project(p, normal));
    }

    friend fspoint<Type, indexer, Size> project(const fspoint<Type, indexer, Size> &a, const fspoint<Type, indexer, Size> &b, const bool normal = false)
    //No sentido em que pode ser mais claro fazer-se
    //project(vector1, vector2) do que vector1.project(vector2),
    //dá-se ao utilizador a possibilidade de escrever de ambas as maneiras...
    {
      return a.project(b, normal);
    }

    friend fspoint<Type, indexer, Size> ortog(const fspoint<Type, indexer, Size> &a, const fspoint<Type, indexer, Size> &b, const bool normal = false)
    //No sentido em que pode ser mais claro fazer-se
    //ortog(vector1, vector2) do que vector1.ortog(vector2),
    //dá-se ao utilizador a possibilidade de escrever de ambas as maneiras...
    {
      return a.ortog(b, normal);
    }

    Type angle(const fspoint<Type, indexer, Size> &p) const
    //Dá o ângulo entre os dois vectores
    {
      Type ret = 0, l1 = 0, l2 = 0;
      indexer i;
      for(i = 0; i < Size; ++i)
        {
          ret += val[i]*p.val[i];
          l1 += val[i]*val[i];
          l2 += p.val[i]*p.val[i];
        }
      return std::acos(ret/sqrt(l1*l2));
    }

    friend Type angle(const fspoint<Type, indexer, Size> &v, const fspoint <Type, indexer, Size> &u)
    //Mais uma vez, para conforto e facilidade de uso.
    {
      return v.angle(u);
    }

    Type cosangle(const fspoint<Type, indexer, Size> &p) const
    //Dá o co-seno do ângulo entre os dois vectores.
    {
      Type ret = 0, l1 = 0, l2 = 0;
      indexer i;
      for(i = 0; i < Size; ++i)
        {
          ret += val[i]*p.val[i];
          l1 += val[i]*val[i];
          l2 += p.val[i]*p.val[i];
        }
      return (ret/sqrt(l1*l2));
    }

    friend Type cosangle(const fspoint<Type, indexer, Size> &v, const fspoint <Type, indexer, Size> &u)
    //Mais uma vez, para conforto e facilidade de uso.
    {
      return v.cosangle(u);
    }

    friend fspoint<Type, indexer, Size> operator*(const Type &a, const fspoint<Type, indexer, Size> &p)
    //Uma função que permite multiplicar um escalar por um vector,
    //pelo simples motivo de que, em certos casos, pode ser útil
    //escrever "escalar" * "vector" em vez do contrário.
    {
      return p*a;
    }

    Type * get_access_to_array()
    //Apenas e só para a implementação em CUDA..
    {
      return val;
    }

    void raw_read(std::istream &s)
    //Lê um point a partir de uma stream.
    {
      indexer i;
      fspoint<Type, indexer, Size> ret;
      for (i = 0; i < Size; ++i)
        {
          s >> ret.val[i];
        }
      (*this) = ret;
    }

  };

  template <class Type, class indexer = size_t, indexer Size = 3> fspoint<Type, indexer, Size> raw_read_point (std::istream &s)
  //Cria um point lido a partir de uma stream,
  //com um número conhecido de elementos (num).
  {
    indexer i;
    fspoint<Type, indexer, Size> ret;
    for (i = 0; i < Size; ++i)
      {
        s >> ret[i];
      }
    return ret;
  }
}

namespace g24_lib
{
  template <class base_T, class exp_T = size_t> CUDA_HOS_DEV base_T fastpow(const base_T &base, exp_T exp)
  //Implementação (expectavelmente eficiente)
  //da exponenciação por números inteiros.
  //Pode vir a ser útil para cálculos diversos...
  {
    base_T r = (base_T) 1, v = base;
    while (exp)
      {
        if (exp & 1)
          {
            r = r*v;
          }
        v = (v*v);
        exp = (exp >> 1);
      }
    return r;
  }

  template <class Type, class indexer = size_t> struct coll
  //Lida com colecções n-dimensionais de pontos.
  //Nas funções, assume-se que a colecção é não-nula,
  //por uma questão de simplicidade de implementação.
  {
  private:
    std::vector<Type*> border;
    std::vector<bool> is_border;
    Type *a;
    //O array que contém os dados.
    indexer dim, Size;
    //O número de dimensões e o número total de elementos do array.
    point<indexer, indexer> nums;
    //O número de pontos por dimensão.
    point<indexer, indexer> sep;
    //O incremento correspondente a cada coordenada.
  public:
    indexer dimension() const
    {
      return dim;
    }

    indexer size() const
    {
      return Size;
    }

    point<indexer, indexer> numbers() const
    {
      return nums;
    }

    point<indexer, indexer> separation() const
    {
      return sep;
    }

    coll()
    //Construtor por defeito.
    {
      dim = 0;
      Size = 0;
      a = nullptr;
    }
    coll(const indexer d, const indexer n, const Type &def):
      //Construtor que inicializa um conjunto de n^d elementos
      //a d dimensões, havendo n elementos por dimensão,
      //com cada elemento correspondente a def.
      nums(d, n),
                                                            sep(d)
    {
      indexer i, j;
      Size = fastpow(n,d);
      dim = d;
      j = Size;
      for (i = 0, j = Size; i < dim; ++i)
        {
          j /= n;
          sep[i] = j;
        };
      a = new Type[Size];
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (indexer k = 0; k < Size; ++k)
        {
          a[k] = def;
        }
    }
    coll(const indexer d, const point<indexer, indexer> &ns, const Type &def) :
      //Construtor que inicializa um conjunto de ns[i] elementos
      //por cada dimensão, com d dimensões no total e cada
      //elemento correspondente a def.
      nums(ns),
                                                                               sep(d)
    {
      indexer i, j;
      dim = d;
      Size = 1;
      for (i = 0; i < d; ++i)
        {
          Size *=ns[i];
        };
      for (i = 0, j = Size; i < dim; ++i)
        {
          j /= ns[i];
          sep[i] = j;
        };
      a = new Type[Size];
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (indexer k = 0; k < Size; ++k)
        {
          a[k] = def;
        }
    }
    coll(const coll &c):
      //Construtor por cópia.
      nums(c.nums),
                        sep(c.sep),
                        border(c.border),
                        is_border(c.is_border)
    {
      Size = c.Size;
      dim = c.dim;
      a = new Type[c.Size];
      memcpy(a, c.a, (c.Size) * sizeof(Type));
    }

    coll& operator= (const coll& c)
    //Operador de assignment.
    {
      if(&c == this)
        {
          return (*this);
        }
      nums = c.nums;
      sep = c.sep;
      border = c.border;
      is_border = c.is_border;
      Size = c.Size;
      dim = c.dim;
      delete[] a;
      a = new Type[c.Size];
      memcpy(a, c.a, (c.Size) * sizeof(Type));
      return (*this);
    }
    Type& operator[](const indexer i)
    //Para aceder directamente aos elementos do array...
    {
      if (i >= Size)
        {
          std::cerr << "\n Trying to access undefined element (" << i << ") of collection!\n" << std::endl;
          return a[Size - 1];
        }
      return a[i];
    }

    Type& operator() (const point<indexer, indexer> p)
    //Para aceder ordenadamente aos elementos do array,
    //embora com maior custo computacional.
    {
      return (*this)[to_elem(p)];
    }

    point<indexer, indexer> to_point(const indexer elem)
    //Converte de uma posição geral
    //para uma posição ordenada.
    {
      indexer j, m = elem;
      point<indexer, indexer> res(dim);
      for (j = 0; j < dim; ++j)
        {
          res[j] = m/sep[j];
          m = m % sep[j];
        }
      return res;
    }

    indexer to_elem(const point<indexer, indexer> &p)
    //Converte de uma posição ordenada
    //para uma posição geral.
    {
      indexer i, elem = 0;
      for (i = 0; i < dim; ++i)
        {
          elem += p[i]*sep[i];
        }
      return elem;
    }

    std::vector<Type> get_near(const point<indexer, indexer> &p)
    //Obtém os pontos mais próximos do elemento.
    {
      return get_near(to_elem(p), p);
    }

    std::vector<Type> get_near(const indexer elem)
    //Obtém os pontos mais próximos do elemento.
    {
      return get_near(elem, to_point(elem));
    }

    std::vector<Type> get_near(const indexer elem, const point<indexer, indexer> &p)
    //Obtém os pontos mais próximos do elemento.
    //(É esta função que faz efectivamente as contas.)
    //O retorno (para um ponto de coordenadas genéricas (a,b,...,d))
    //estará ordenado da seguinte forma:
    //coll(a+1,b,...,d) coll(a-1,b,...,d) coll(a,b+1,...,d) ...
    //Sendo omitidos os elementos sem significado (isto é,
    //para pontos na fronteira...).
    {
      indexer i, j = 0;
      std::vector<Type> ret;
      ret.reserve(dim*2);
      for (i = 0; i < dim; ++i)
        {
          if (p[i] > 0)
            {
              ret.push_back(a[elem + sep[i]]);
              ++j;
            }
          if (p[i] && p[i] < nums[i] - 1)
            {
              ret.push_back(a[elem - sep[i]]);
              ++j;
            }
        }
      ret.shrink_to_fit();
      return ret;
    }
    std::vector<Type *> & get_border()

    //NOTA: o algoritmo inerente a esta função
    //foi (dolorosamente) desenvolvido pelos autores.
    //Desconhemos se é sobejamente conhecido ou não,
    //mas consideramo-lo de vaga relevância, no sentido
    //em que dificilmente poderá ser melhorado, posto que
    //já é linear

    {
      if (border.size())
        {
          return border;
        }
      is_border.resize(Size, false);
      border.reserve(Size);
      //Poder-se-ia fazer uma estimativa um pouco mais
      //fina da fronteira da colecção, com base nos produtos
      //alternados do número de componentes em cada direcção,
      //mas, uma vez que o supremo do número de elementos da fronteira
      //no caso geral acaba por ser o tamanho da colecção,
      //adoptou-se essa estimativa.
      indexer n = 0;
      for (indexer i = 0; i < dim; ++i)
        {
          for (indexer k = 0; k < sep[i]; ++k)
            {
              is_border[k+n] = true;
              border.push_back(&a[k+n]);
              is_border[k+n+(nums[i]-1)*sep[i]] = true;
              border.push_back(&a[k+n+(nums[i]-1)*sep[i]]);
            }
          if (i > 0)
            {
              n += sep[i-1];
              if (n >= Size)
                {
                  n -= sep[i-1];
                }
              else
                {
                  --i;
                }
            }
        }
      border.shrink_to_fit();
      return border;
    }
    bool get_is_border(point <indexer, indexer> &p)
    //Verifica se um elemento numa dada posição está na fronteira.
    {
      if (border.size())
        {
          return get_is_border(to_elem(p));
        }
      indexer i;
      //Neste caso, por se tratar de um ponto,
      //é mais rápido verificar se está na fronteira
      //pelas coordenadas do que estar a encontrar
      //a totalidade da fronteira, nos casos em que
      //já não se o tenha feito.
      for(i = 0; i < dim; ++i)
        {
          if (p[i] == 0 || p[i] == nums[i] - 1)
            {
              return true;
            }
        }
      return false;
    }

    bool get_is_border(indexer elem)
    {
      if (border.size())
        {
          return is_border[elem];
        }
      get_border();
      return is_border[elem];
    }

    void set_border(const Type &v)
    //Coloca a fronteira com um valor específico...
    {
      if (!border.size())
        {
          get_border();
        }
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (indexer i = 0; i < border.size(); ++i)
        {
          *border[i] = v;
        }
    }

    void operate_on_all(Type (*f) (const Type &, const indexer, const coll<Type, indexer> &, void*), void *par = nullptr)
    //Opera sobre todos os elementos da colecção.
    //O argumento adicional será um eventual argumento
    //para a função efectuada, que, além desse,
    //receberá o índice do elemento na colecção.
    {
#ifdef _OPENMP
#pragma omp parallel for
#endif
      for (indexer i = 0; i < Size; ++i)
        {
          a[i] = f(a[i], i, (*this), par);
        }
    }

    void operate_on_all(Type (*f) (const Type &, const point<indexer, indexer> &, const coll<Type, indexer> &, void *), void *par = nullptr)
    //Opera sobre todos os elementos da colecção.
    //O argumento adicional será um eventual argumento
    //para a função efectuada, que, além desse,
    //receberá as coordenadas do elemento na colecção.
    {
#ifdef _OPENMP
#pragma omp parallel for
      for (indexer i = 0; i < Size; ++i)
        {
          point<indexer, indexer> p = to_point (i);
          a[i] = f(a[i], p, (*this), par);
        }
#else
      point<indexer, indexer> p(dim,0);
      indexer i;
      for (i = 0; i < Size; ++i)
        {

          a[i] = f(a[i], p, (*this), par);
          p.inc_with_wrap(nums, dim-1, true, true);
        }
#endif
    }

    ~coll()
    //Destructor...
    {
      delete[] a;
    }

    std::vector<bool>& get_access_to_is_border()
    //Função específica para CUDA...
    {
      return is_border;
    }

  };

  template <typename Block, typename indexer = size_t> struct carried_bool_vector
  //Implementação simples de std::vector<bool>,
  //de modo a providenciar um array copiável para a GPU...
  {
    static indexer chunk;
    //O número de elementos (bits) em cada bloco.
    indexer Size, numb;
    //O número total de elementos contidos
    //e o número total de blocos.
    Block *a;
    carried_bool_vector(const std::vector<bool> v)
    {
      Size = v.size();
      numb = v.size()/chunk + (v.size() % chunk != 0);
      //Na última parte, se x%y for zero, dá 0, se não, dá 1...
      a = new Block[numb];
      indexer i, j;
      for(i = 0; i < numb; ++i)
        {
          a[i] = 0;
          if(i == numb - 1)
            {
              for(j = 0; chunk*i+j < Size; ++j)
                {
                  a[i] |= ((Block) v[chunk*i + j]) << j;
                }
            }
          else
            {
              for(j = 0; j < chunk; ++j)
                {
                  a[i] |= ((Block) v[chunk*i + j]) << j;
                }
            }
        }
    }
    bool operator[] (const indexer i) const
    //Acede ao i-ésimo elemento do vector
    {
      return (bool) (a[i/chunk] & (Block) 1 << i%chunk);
    }
    Block& operator() (const indexer i)
    //Acede, pura e simplesmente, ao i-ésimo bloco portador.
    {
      return (a[i/chunk]);
    }
    Block operator() (const indexer i) const
    //Acede, pura e simplesmente, ao i-ésimo bloco portador.
    //(Sem permitir alterações do valor, claro está...)
    {
      return (a[i/chunk]);
    }
    void write (const indexer i, const bool b)
    //Escreve no i-ésimo elemento do vector
    //o valor de b
    {
      a[i/chunk] = (a[i/chunk] | (Block) ((Block) b << i%chunk)) & (Block) ~(! ((Block) b) << i%chunk);
      //Pela álgebra booleana, deve funcionar...
    }
    ~carried_bool_vector()
    {
      delete[] a;
    }
  };

  template <typename Block, typename indexer> indexer carried_bool_vector<Block, indexer>::chunk = 8*sizeof(Block);

  template <class Type> CUDA_HOS_DEV Type sign(Type num)
  //Retorna o sinal de uma qualquer coisa.
  {
    return (Type) ((((Type) 0) < num) - (num < ((Type) 0)));
  }

  template <class Type> CUDA_HOS_DEV Type stand(const Type &x)
  //Retorna um valor no intervalo ]-1, 1]
  {
    return (Type) x + 2.0*std::floor(0.5-0.5*x);
  }

  //Seguem-se umas forward declarations úteis...

  template <class Type> class comp;
  template <class Type> comp<Type> read_comp(std::istream &s, const bool reduc = false);
  template <class Type> comp<Type> read_comp(std::istream &s, comp<Type> &c, const bool reduc = false);
  template <class Type> comp<Type> ask_comp(const bool reduc = false);
  template <class Type> comp<Type> fast_read_comp(std::istream &s, const bool pol = false, const bool reduc = false);
  template <class Type> comp<Type> fast_read_comp(std::istream &s, comp<Type> &c, const bool pol = false, const bool reduc = false);

  //E retomamos o código normal...


  template <class Type> class comp
  //Trabalha com complexos, de uma maneira geral.
  //Está como template sobretudo para poder ser usada
  //com floats e doubles, embora seja possível
  //utilizar outros tipos, com resultados expectavelmente
  //estranhos se forem usados, por exemplo, tipos unsigned...
  {
  private:
    Type rho, theta;
    //Forma polar; theta em ]-1, 1], uma vez que se multiplica por pi.
    Type re, im;
    //Forma cartesiana.

    CUDA_HOS_DEV void update_polar()
    //Actualiza a representação na forma polar.
    {
      rho = (Type) sqrt(re*re + im*im);
      theta = (Type) atan2(im, re)/(M_PI);
    }
    CUDA_HOS_DEV void update_cart()
    //Acutaliza a representação na forma cartesiana.
    {
      re = rho*cos(theta*M_PI);
      im = rho*sin(theta*M_PI);
    }
  public:
    CUDA_HOS_DEV comp()
    //Construtor por defeito.
    {
      re = (Type) 0;
      im = (Type) 0;
      rho = (Type) 0;
      theta = (Type) 0;
    }
    CUDA_HOS_DEV comp(const Type &x)
    //Construtor com base num número real.
    {
      re = x;
      im = (Type) 0;
      rho = (Type) fabs(x);
      theta = (Type) 0.5 - 0.5*sign(x);
    }
    CUDA_HOS_DEV comp (const Type &x, const Type &y, const bool pol = false)
    //Construtor com base em dois números.
    //Caso se dê um terceiro argumento como verdadeiro,
    //os valores dados são interpretados como a representação polar.
    {
      if(!pol)
        {
          re = x;
          im = y;
          update_polar();
        }
      else
        {
          rho = x;
          theta = stand(y);
          update_cart();
        }
    }
    CUDA_HOS_DEV comp (const point<Type> &p, const bool pol = false)
    //Construtor com base num vector de comprimento pelo menos 2.
    //Caso se dê um segundo argumento como verdadeiro,
    //os valores dados são interpretados como a representação polar.
    {
      if(!pol)
        {
          re = p[0];
          im = p[1];
          update_polar();
        }
      else
        {
          rho = (Type) fabs(p[0]);
          theta = stand(p[1]);
          update_cart();
        }
    }

    CUDA_HOS_DEV comp (const Type x, const Type y, const Type r, const Type t)
    //Construtor que parte da totalidade dos elementos usados
    //para representar um número complexo...
    //Psoto que é suposto ser usado sobretudo para as operações
    //com complexos cuja expressão analítica é conhecida,
    //não se implementam quaisquer verificações, por uma questão
    //de rapidez e eficiência...
    {
      re = x;
      im = y;
      rho = r;
      theta = t;
    }

    CUDA_HOS_DEV comp (const comp &z)
    {
      re = z.re;
      im = z.im;
      rho = z.rho;
      theta = z.theta;
    }

    CUDA_HOS_DEV comp operator+ (const comp &z) const
    //Soma complexos.
    {
      return comp<Type>(re+z.re, im+z.im);
    }

    CUDA_HOS_DEV comp operator+ (const Type &x) const
    //Soma escalares a complexos.
    {
      return comp<Type>(re+x, im);
    }

    friend CUDA_HOS_DEV comp<Type> operator+ (const Type &x, const comp<Type> &z)
    //Soma complexos a escalares.
    {
      return comp<Type>(z.re+x, z.im);
    }

    CUDA_HOS_DEV comp operator- (const comp &z) const
    //Subtrai complexos.
    {
      return comp<Type>(re-z.re, im-z.im);
    }

    CUDA_HOS_DEV comp operator- (const Type &x) const
    //Subtrai escalares a complexos.
    {
      return comp<Type>(re-x, im);
    }

    friend CUDA_HOS_DEV comp<Type> operator- (const Type &x, const comp<Type> &z)
    //Subtrai complexos a escalares.
    {
      return comp<Type>(x-z.re, -z.im);
    }

    CUDA_HOS_DEV comp operator- () const
    //Inverte o sinal de um complexo.
    {
      return comp<Type>(-re, -im, rho, stand(theta + 1.0));
    }

    CUDA_HOS_DEV comp operator* (const comp &z) const
    //Multiplica complexos.
    {
      return comp<Type>(re*z.re-im*z.im, re*z.im+im*z.re, rho*z.rho, stand(theta+z.theta));
    }

    CUDA_HOS_DEV comp operator* (const Type &s) const
    //Multiplica complexos por escalares.
    {
      return comp<Type>(s*re, s*im, (Type) fabs(s)*rho, stand(sign(s)*theta));
    }

    friend CUDA_HOS_DEV comp<Type> operator* (const Type &s, const comp<Type> &z)
    //Multiplica escalares por complexos.
    {
      return comp<Type>(s*z.re, s*z.im, (Type) fabs(s)*z.rho, stand(sign(s)*z.theta));
    }

    CUDA_HOS_DEV comp operator/ (const comp &z) const
    //Divide complexos.
    {
      return comp<Type>((re*z.re+im*z.im)/(z.rho*z.rho), (im*z.re-re*z.im)/(z.rho*z.rho), rho/z.rho, stand(theta-z.theta));
    }

    CUDA_HOS_DEV comp<Type> operator/ (const Type &s) const
    //Divide complexos por escalares.
    {
      return comp<Type>(re/s , im/s , (Type) rho/fabs(s), stand(sign(s)*theta));
    }

    friend CUDA_HOS_DEV comp<Type> operator/ (const Type &s, const comp<Type> &z)
    //Divide escalares por complexos.
    {
      return comp<Type>((s*z.re)/(z.rho*z.rho), (-s*z.im)/(z.rho*z.rho), (Type) fabs(s)/z.rho, stand((Type) sign(s) - z.theta));
    }

    CUDA_HOS_DEV comp & operator= (const comp &z)
    //Atribui um complexo a outro
    {
      re = z.re;
      im = z.im;
      rho = z.rho;
      theta = z.theta;
      return (*this);
    }

    CUDA_HOS_DEV comp & operator+= (const comp &z)
    //Atribui complexo à sua soma com outro.
    {
      re +=z.re;
      im += z.im;
      update_polar();
      return (*this);
    }

    CUDA_HOS_DEV comp & operator-= (const comp &z)
    //Atribui complexo à sua subtracção por outro.
    {
      re -= z.re;
      im -= z.im;
      update_polar();
      return (*this);
    }

    CUDA_HOS_DEV comp & operator*= (const comp &z)
    //Atribui complexo ao seu produto por outro.
    {
      comp<Type> temp;
      temp.re = re*z.re-im*z.im;
      temp.im = re*z.im+im*z.re;
      temp.rho = rho*z.rho;
      temp.theta = stand(theta+z.theta);
      (*this) = temp;
      return (*this);
    }

    CUDA_HOS_DEV comp & operator/= (const comp &z)
    //Atribui complexo à sua divisão por outro.
    {
      comp<Type> temp;
      temp.re = (re*z.re+im*z.im)/(z.rho*z.rho);
      temp.im = (im*z.re-re*z.im)/(z.rho*z.rho);
      temp.rho = rho/z.rho;
      temp.theta = stand(theta-z.theta);
      (*this) = temp;
      return (*this);
    }


    CUDA_HOS_DEV bool operator == (const comp &z) const
    //Verifica a igualdade de complexos.
    {
      return (re == z.re && im == z.im);
      //Pela ambiguidade do argumento quando o módulo é zero,
      //faz-se a comparação com a representação cartesiana...
    }

    CUDA_HOS_DEV bool checkequal(const comp &z, double epsilon = std::numeric_limits<double>::epsilon()) const
    //Verifica, dentro de uma tolerância, a igualdade de complexos.
    {
      return (fabs(re - z.re) < epsilon && fabs(im - z.im) < epsilon);
    }

    CUDA_HOS_DEV bool operator != (const comp &z) const
    //Verifica a diferença de complexos.
    {
      return (re != z.re || im != z.im);
    }

    CUDA_HOS_DEV bool checkdiff(const comp &z, double epsilon = std::numeric_limits<double>::epsilon()) const
    //Verifica, dentro de uma tolerância, a diferença de complexos.
    {
      return (fabs(re - z.re) >= epsilon || fabs(im - z.im) >= epsilon);
    }

    CUDA_HOS_DEV Type real() const
    //Parte real
    {
      return re;
    }

    CUDA_HOS_DEV Type imag() const
    //Parte imaginária
    {
      return im;
    }

    CUDA_HOS_DEV Type abs() const
    //Módulo
    {
      return rho;
    }

    CUDA_HOS_DEV Type arg(const bool reduce = false) const
    //Argumento em ]-pi, pi];
    //se dado um argumento verdadeiro,
    //retorna em ]-1, 1].
    {
      if(reduce)
        {
          return theta;
        }
      else
        {
          return theta*M_PI;
        }
    }

    CUDA_HOS_DEV comp conjug() const
    //Conjugado
    {
      return comp<Type>(re, -im, rho, -theta);
    }

    friend CUDA_HOS_DEV comp<Type> log(const comp<Type> &z)
    //Logaritmo complexo
    {
      return comp <Type> (log(z.rho), z.theta, false);
    }

    friend CUDA_HOS_DEV comp<Type> exp(const comp<Type> &z)
    //Exponencial complexa
    {
      return comp <Type> (exp(z.re)*cos(z.im), exp(z.re)*sin(z.im), exp(z.re), stand(z.im));
    }

    static comp<Type> I_;
    //A unidade imaginária.

    friend CUDA_HOS_DEV comp<Type> sin(const comp<Type> &z)
    //O seno.
    {
      return (exp(I_*z) - exp(-I_*z))/(((Type) 2)*I_);
    }

    friend CUDA_HOS_DEV comp<Type> cos(const comp<Type> &z)
    //O co-seno.
    {
      return (exp(I_*z) + exp(-I_*z))/((Type) 2);
    }

    friend CUDA_HOS_DEV comp<Type> tan(const comp<Type> &z)
    //A tangente.
    {
      return (exp(I_*z) - exp(-I_*z))/(I_*(exp(I_*z) + exp(-I_*z)));
    }

    friend CUDA_HOS_DEV Type fabs(const comp<Type> &z)
    //O módulo, como função e não método.
    {
      return z.rho;
    }

    friend CUDA_HOS_DEV Type abs(const comp<Type> &z)
    //Outra forma de obter o módulo como função...
    {
      return z.rho;
    }
    friend std::istream& operator>> (std::istream &s, comp<Type> &z)
    //Forma padrão (cartesiana) de ler complexos.
    {
      s >> z.re;
      s >> z.im;
      z.update_polar();
      return s;
    }
    friend std::ostream& operator<< (std::ostream &s, const comp<Type> &z)
    //Forma padrão (cartesiana) de escrever complexos.
    {
      s << z.re << " " << z.im;
      return s;
    }
    friend void print_comp(std::ostream &s, const comp<Type> &z, const bool pol = false, const bool reduc = false)
    //Imprime complexos. Se pol for falso, imprime
    //na forma cartesiana. Se pol for verdadeiro,
    //imprime na forma polar, usando o argumento
    //reduzido (como fracção de pi) se reduc for verdadeiro.
    {
      if (!pol)
        {
          s << z.re;
          if (sign(z.im) < 0)
            {
              s << " - " << fabs(z.im) << " i";
            }
          else if (sign(z.im) > 0)
            {
              s << " + " << fabs(z.im) << " i";
            }
        }
      else
        {
          s << z.rho << " cis ";
          if (reduc)
            {
              s << z.theta;
            }
          else
            {
              s << z.theta*M_PI;
            }
        }
    }
  private:
    void parse_comp(std::istream &s, const bool reduc = false)
    //Interpreta um complexo de uma stream.
    //Se o booleano opcional for verdadeiro,
    //caso se determine que o complexo está na
    //forma polar, o argumento é lido como fracção de pi.
    //
    //Reconhece os inputs do tipo:
    //
    //a + ib, a - ib, a + bi, a - bi,
    //bi + a, bi, a
    //rho cis theta, rho pol theta
    //
    //Ignorando whitespace e capitalização.
    {
      double temp1;
      //Double para receber números fraccionais...
      char u;
      s >> temp1;
      u = s.peek();
      if(u == '\n' || u == '\0' || s.eof())
        {
          re = (Type) temp1;
          im = (Type) 0;
          rho = (Type) ::fabs(temp1);
          theta = (Type) 0.5 - 0.5*sign(temp1);
          return;
        }
      s >> std::ws;
      u = s.peek();
      if(u == '+')
        {
          s >> u >> std::ws;
          re = (Type) temp1;
          u = s.peek();
          if(u == 'i' || u == 'I')
            {
              s >> u >> temp1;
              im = (Type) temp1;
              update_polar();
            }
          else if (std::isdigit(u))
            {
              s >> temp1;
              u = s.peek();
              im = (Type) temp1;
              update_polar();
              if(u != 'i' && u != 'I')
                {
                  s.setstate(std::ios::failbit);
                }
            }
        }
      else if(u == '-')
        {
          re = temp1;
          s >> u >> std::ws;
          u = s.peek();
          if(u == 'i' || u == 'I')
            {
              s >> u >> temp1;
              im = - (Type) temp1;
              update_polar();
              return;
            }
          else if (std::isdigit(u))
            {
              s >> temp1;
              u = s.peek();
              im = (Type) -temp1;
              update_polar();
              if(u != 'i' || u != 'I')
                {
                  s.setstate(std::ios::failbit);
                }
            }
        }
      else if(u == 'c' || u == 'C')
        //Seria, talvez, mais compacto
        //usar uma string, mas assim
        //damos a possibilidade de
        //não haver whitespace...
        {
          s >> u >> u;
          if(u == 'i' || u == 'I')
            {
              s >> u;
              if(u == 's' || u == 'S')
                {
                  rho = (Type) ::fabs(temp1);
                  if(temp1 < 0)
                    {
                      theta = (Type) 1;
                    }
                  else
                    {
                      theta = (Type) 0;
                    }
                  s >> temp1;
                  if(!reduc)
                    {
                      temp1 = (Type) temp1/M_PI;
                    }
                  theta = stand(theta + temp1);
                  update_cart();
                }
            }
        }
      else if(u == 'p' || u == 'P')
        {
          s >> u >> u;
          if(u == 'o' || u == 'O')
            {
              s >> u;
              if(u == 'l' || u == 'L')
                {
                  rho = (Type) ::fabs(temp1);
                  if(temp1 < 0)
                    {
                      theta = (Type) 1;
                    }
                  else
                    {
                      theta = (Type) 0;
                    }
                  s >> temp1;
                  if(!reduc)
                    {
                      temp1 = (Type) temp1/M_PI;
                    }
                  theta = stand(rho + temp1);
                  update_cart();
                }
            }
        }
      else if(u == 'i' || u == 'I')
        {
          im = (Type) temp1;
          s >> u;
          u = s.peek();
          if(u == '\n' || u == '\0' || s.eof())
            {
              re = (Type) 0;
              rho = (Type) fabs(im);
              theta = 0.5*sign(im);
              return;
            }
          s >> std::ws;
          u = s.peek();
          if(u == '-')
            {
              s >> u >> temp1;
              re = (Type) -temp1;
            }
          else if(u == '+')
            {
              s >> u >> temp1;
              re = (Type) temp1;
            }
          update_polar();
        }
      else
        {
          s.setstate(std::ios::failbit);
        }
    }

  public:

    CUDA_HOS_DEV ~comp()
    {
      return;
    }
    //O destructor nada necessita de fazer,
    //visto que não há alocações dinâmicas.

    friend comp<Type> read_comp<Type>(std::istream &s, const bool reduc);
    friend comp<Type> read_comp<Type>(std::istream &s, comp<Type> &c, const bool reduc);
    friend comp<Type> ask_comp<Type>(const bool reduc);
    friend comp<Type> fast_read_comp<Type>(std::istream &s, const bool pol, const bool reduc);
    friend comp<Type> fast_read_comp<Type>(std::istream &s, comp<Type> &c, const bool pol, const bool reduc);
  };

  template <class Type> comp<Type> comp<Type>::I_((Type) 0, (Type) 1, (Type) 1, (Type) 0.5);


  template <class Type> comp<Type> read_comp(std::istream &s, const bool reduc)
  //Lê um complexo de uma stream.
  //Se se der um argumento verdadeiro adicional,
  //caso o complexo esteja na forma polar,
  //o ângulo é lido na forma reduzida (fracção de pi).
  {
    comp<Type> temp;
    temp.parse_comp(s, reduc);
    return temp;
  }
  template <class Type> comp<Type> read_comp(std::istream &s, comp<Type> &c, const bool reduc)
  //Actualiza um complexo com os dados de uma stream.
  //Se se der um argumento verdadeiro adicional,
  //caso o complexo esteja na forma polar,
  //o ângulo é lido na forma reduzida (fracção de pi).
  {
    c.parse_comp(s, reduc);
    return c;
  }

  template <class Type> comp<Type> ask_comp(const bool reduc)
  //Cria um complexo definido pelo utilizador.
  //Se se der um argumento verdadeiro adicional,
  //caso o complexo esteja na forma polar,
  //o ângulo é lido na forma reduzida (fracção de pi).
  {
    comp<Type> temp;
    std::cout << "Por favor, introduza um número complexo. " << std::flush;
    read_comp(std::cin, temp, reduc);
    return temp;
  }
  template <class Type> comp<Type> fast_read_comp(std::istream &s, const bool pol, const bool reduc)
  //Cria um complexo lido de uma stream.
  //Se se der um argumento verdadeiro,
  //é lido um complexo na forma polar.
  //Se se der um argumento verdadeiro adicional,
  //caso o complexo esteja na forma polar,
  //o ângulo é lido na forma reduzida (fracção de pi).
  {
    Type temp1, temp2;
    s >> temp1 >> temp2;
    if (reduc)
      {
        return (comp<Type>) (temp1, M_PI*temp2, pol);
      }
    return (comp<Type>) (temp1, temp2, pol);
  }
  template <class Type> comp<Type> fast_read_comp(std::istream &s, comp<Type> &c, const bool pol, const bool reduc)
  //Lê de uma stream directamente para um complexo instanciado.
  //Se se der um argumento verdadeiro, é lido um complexo na
  //forma polar. Caso se dê, adicionalmente, um outro argumento verdadeiro,
  //o ângulo é interpretado na forma reduzida (ou seja, dividido por pi...)
  {
    if (pol)
      {
        s >> c.rho >> c.theta;
        c.theta = stand((reduc ? c.theta : c.theta/M_PI));
        c.update_cart();
      }
    else
      {
        s >> c.re >> c.im;
        c.update_polar();
      }
    return c;
  }

  //Estas duas últimas funções correspondem a versões mais ligeiras
  //da função de leitura genérica, por permitirem saltar o passo
  //de interpretação do que foi escrito...


  template <class hist_record, class indexer = size_t> struct history
  //hist_record tem de ter funções void initialize() e bool initialized(),
  //a primeira inicializando-o para um valor nulo,
  //a segunda verificando se está ou não inicializado,
  //bem como um operator= em relação a instâncias da classe...
  {
  private:
    hist_record **a;
    indexer numel, numr;
    indexer newest, oldest;
  public:

    indexer get_newest() const
    {
      return newest;
    }

    hist_record& get_newest_record(const indexer i)
    {
      return a[i][newest];
    }

    hist_record get_newest_record(const indexer i) const
    {
      return a[i][newest];
    }

    hist_record& get_oldest_record(const indexer i)
    {
      return a[i][oldest];
    }

    hist_record get_oldest_record(const indexer i) const
    {
      return a[i][oldest];
    }

    indexer get_oldest() const
    {
      return oldest;
    }

    indexer get_numr(const bool all = false) const
    {
      if (all)
        {
          return numr;
        }
      else
        {
          return (newest < oldest ? numr : (!a[0][0].initialized() ? newest + 1 : 0) );
        }
    }

    indexer get_numel() const
    {
      return numel;
    }

    void initialize(const indexer num)
    {
      indexer i;
      for(i = 0; i < numr; ++i)
        {
          a[num][i].initialize();
        }
    }

    void initialize_all()
    {
      newest = 0;
      oldest = 0;
      indexer i;
      for(i = 0; i < numel; ++i)
        {
          initialize(i);
        }
    }

    void rearrange()
    {
      indexer i;
      hist_record *temp;
      for(i = 0; i < numel; ++i)
        {
          temp = new hist_record[numr];
          std::memcpy(&temp[0], &a[i][oldest], (numr-oldest)*sizeof(hist_record));
          std::memcpy(&temp[numr-oldest], &a[i][0], (oldest)*sizeof(hist_record));
          delete[] a[i];
          a[i] = temp;
        }
    }

    history()
    {
      a = nullptr;
      numel = 0;
      numr = 0;
      newest = 0;
      oldest = 0;
    }

    history(const indexer q, const indexer r)
    {
      indexer i;
      a = new hist_record*[q];
      for(i = 0; i < q; ++i)
        {
          a[i] = new hist_record[r];
        }
      numel = q;
      numr = r;
      initialize_all();
    }

    history(const history& h)
    {
      indexer i;
      numel = h.numel;
      numr = h.numr;
      newest = h.newest;
      oldest = h.oldest;
      if(h.a)
        {
          a = new hist_record*[numel];
          for(i = 0; i < numel; ++i)
            {
              a[i] = new hist_record[numr];
              std::memcpy(a[i], h.a[i], numr*sizeof(hist_record));
            }
        }
      else
        {
          a = nullptr;
        }
    }

    history& operator = (const history& h)
    {
      indexer i;
      if(&h != this)
        {
          if(a != nullptr)
            for(i = 0; i < numel; ++i)
              {
                delete[] a[i];
              }
          delete[] a;
          numel = h.numel;
          numr = h.numr;
          newest = h.newest;
          oldest = h.oldest;
          if(h.a != nullptr)
            {
              a = new hist_record*[numel];
              for(i = 0; i < numel; ++i)
                {
                  a[i] = new hist_record[numr];
                  std::memcpy(a[i], h.a[i], numr*sizeof(hist_record));
                }
            }
          else
            {
              a = nullptr;
            }
        }
      return (*this);
    }

    void clear()
    {
      indexer i;
      if (a!= nullptr)
        {
          for(i = 0; i < numel; ++i)
            {
              delete[] a[i];
            }
          delete[] a;
          a = nullptr;
        }
      numel = 0;
      numr = 0;
      newest = 0;
      oldest = 0;
    }

    void add_element(const indexer elnm = 1)
    {
      indexer i;
      hist_record **temp = new hist_record*[numel+elnm];
      std::memcpy(temp, a, numel*sizeof(hist_record*));
      if(a != nullptr)
        {
          delete[] a;
        }
      a = temp;
      for(i = 0; i < elnm; ++i)
        {
          a[numel+i] = new hist_record[numr];
          initialize(numel+i);
        }
      numel += elnm;
    }

    history<hist_record, indexer> concatenate(const history<hist_record, indexer> &other) const
    {
      if(numr != other.numr)
        {
          std::cerr << "Incompatible sizes for history concatenation." << std::endl;
          return (*this);
        }
      history<hist_record, indexer> ret;
      hist_record **temp = new hist_record*[numel+other.numel];
      std::memcpy(temp, &a[oldest], (numel-oldest)*sizeof(hist_record*));
      std::memcpy(&temp[numel-oldest], &a[0], (oldest)*sizeof(hist_record*));
      std::memcpy(&temp[numel], &other.a[a.oldest], (other.numel-other.oldest)*sizeof(hist_record*));
      std::memcpy(&temp[numel+other.numel-other.oldest], &other.a[0], (other.oldest)*sizeof(hist_record*));
      ret.a = temp;
      ret.numel = numel + other.numel;
      ret.numr = numr;
      ret.oldest = 0;
      ret.newest = ((oldest > 0 && other.oldest > 0) ?
                    numr-1 : std::min(newest, other.newest) );
      return ret;
    }

    void remove_element(const indexer which = std::numeric_limits<indexer>::max(), const bool can_reorder = false)
    {
      hist_record **temp;
      if(can_reorder)
        {
          delete[] a[which];
          temp = new hist_record*[numel - 1];
          std::memcpy(&a[which], &a[numel-1], sizeof(hist_record*));
          std::memcpy(temp, a, (numel-1)*sizeof(hist_record*));
          delete[] a;
          a = temp;
        }
      else
        {
          if(which < numel)
            {
              delete[] a[which];
              temp = new hist_record*[numel - 1];
              if(which > 0)
                {
                  std::memcpy(temp, a, which*sizeof(hist_record*));
                }
              if(which < numel - 1)
                {
                  std::memcpy(&temp[which], &a[which+1], (numel-which-1)*sizeof(hist_record*));
                }
              delete[] a;
              a = temp;
            }
          else
            {
              delete[] a[numel - 1];
              temp = new hist_record*[numel - 1];
              std::memcpy(temp, a, (numel-1)*sizeof(hist_record*));
              delete[] a;
              a = temp;
            }
        }
      numel -= 1;
    }

    void record(const hist_record * arr)
    {
      indexer i;
      if(numr == 0)
        {
          return;
        }
      if(! (newest == 0 && oldest == 0 && a[0][0].initialized()) )
        {
          newest = (newest+1) % numr;
          if(newest <= oldest)
            {
              oldest = (oldest+1) % numr;
            }
        }
      for(i = 0; i < numel; ++i)
        {
          a[i][newest] = arr[i];
        }
    }

    void resize_records(const indexer new_size)
    {
      if(new_size == numr)
        {
          return;
        }
      indexer i, j;
      hist_record *temp;
      if(new_size > numr)
        {
          for(i = 0; i < numel; ++i)
            {
              temp = new hist_record[new_size];
              if(oldest > newest)
                {
                  std::memcpy(temp, &a[i][oldest], (numr-oldest)*sizeof(hist_record));
                  std::memcpy(&temp[numr-oldest], &a[i][0], oldest*sizeof(hist_record));
                }
              else
                {
                  std::memcpy(temp, &a[i][0], numr*sizeof(hist_record));
                }
              delete[] a[i];
              a[i] = temp;
              for(j = numr; j < new_size; ++j)
                {
                  a[i][j].initialize();
                }
            }
          if(oldest > newest)
            {
              newest = numr-1;
              oldest = 0;
            }
        }
      else //new_size == numr já está tratado...
        {
          if(oldest >= new_size)
            {
              for(i = 0; i < numel; ++i)
                {
                  temp = new hist_record[new_size];
                  std::memcpy(temp, &a[i][oldest - new_size], new_size*sizeof(hist_record));
                  delete[] a[i];
                  a[i] = temp;
                }
              oldest = 0;
              newest = new_size-1;
            }
          else
            {
              for(i = 0; i < numel; ++i)
                {
                  temp = new hist_record[new_size];
                  if(oldest > newest)
                    {
                      std::memcpy(temp,&a[i][oldest+numr-new_size], (new_size-oldest)*sizeof(hist_record));
                      std::memcpy(&temp[new_size-oldest],&a[i][0], (oldest)*sizeof(hist_record));
                    }
                  else if(newest > new_size)
                    {
                      std::memcpy(temp,&a[i][newest-new_size+1], new_size*sizeof(hist_record));
                    }
                  else
                    {
                      std::memcpy(temp,&a[i][0], new_size*sizeof(hist_record));
                    }
                  delete[] a[i];
                  a[i] = temp;
                }
              if(newest > new_size)
                {
                  oldest = 0;
                  newest = new_size-1;
                }
            }
        }
      numr = new_size;
    }
    hist_record& operator() (const indexer n, const indexer m, const bool ordered = true)
    {
      if(ordered)
        {
          return a[n][(m+oldest) % (numr)];
        }
      return a[n][m];
    }

    hist_record operator() (const indexer n, const indexer m, const bool ordered = true) const
    {
      if(ordered)
        {
          return a[n][(m+oldest) % (numr)];
        }
      return a[n][m];
    }

    void reset()
    {
      initialize_all();
    }

    ~history()
    {
      if(a != nullptr)
        {
          indexer i;
          for(i = 0; i < numel; ++i)
            {
              if(a[i] != nullptr)
                {
                  delete[] a[i];
                }
            }
          delete[] a;
        }
    }

  };

  template<class A, class B = size_t> void print_all(const history<A,B>& h, std::ostream &s = std::cout)
  {
    for(size_t i = 0; i < h.get_numel(); ++i)
      {
        s << i << " : ";
        for(size_t j = 0; j < h.get_numr(); ++j)
          {
            s << h(i,j,false) << " ";
          }
        s << std::endl;
      }
  }


  //----------------------------------------------------------------//

  //O código que se segue é retirado mais ou menos directamente
  //de um projecto final de programação, com a única particularidade
  //de terem sido substituídos os ponteiros NULL por nullptr,
  //posto que estes são mais type-safe (e o C++ assim o permite),
  //e a codificação das funções como templates para alternar entre
  //um retorno do tipo float ou double.
  //De resto, usam-se C-style strings, por uma questão de reaproveitamento
  //directo do código anterior, ainda que, naturalmente, com os problemas
  //em termos de bound checking que daí advêm.

#define QE__ 1.6021766208e-19
  //A carga elementar (para questões diversas de mudança de unidades).
#define BEGINMU1 -62
#define ENDMU1 -75
  //Um mu em UTF-8.
#define BEGINMU2 -50
#define ENDMU2 -68
  //Outro mu em UTF-8.
#define BEGINANGS1 -61
#define ENDANGS1 -123
  //Um Angstrom em UTF-8.
#define BEGINANGS2 -30
#define MIDDLEANGS2 -124
#define ENDANGS2 -85
  //Outro Angstrom em UTF-8.


  enum siconvertmode { SIC_PREFIXES, SIC_LENGTH, SIC_TIME,
                       SIC_VELOCITY, SIC_MASS, SIC_ELECTRIC_CHARGE,
                       SIC_ACCELERATION, SIC_LINEAR_MOMENTUM      };


  template <class Type> Type si_conversion (char *s, const siconvertmode mode, const size_t len, char **rest)
  //Lê a string s, de comprimento len, e reconhece (algumas) unidades e prefixos.
  //"mode" indica as dimensões que se pretende reconhecer e rest é um pointer
  //para uma C-style string que conterá o resto dos caracteres (não reconhecidos)
  //O argumento da template (Type) serve, essencialmente,
  //para trocar entre floats e doubles. O uso de outros tipos
  //seria mais estranho, podendo levar a resultados sem significado...
  //
  //Convém ressalvar-se que, um pouco em contradição com as melhores
  //práticas da programação, surgem aqui múltiplos "magic numbers",
  //isto é, colocou-se os valores para a conversão directamente no
  //código sem indicar propriamente o que significam, mas, pelo contexto,
  //acabarão por ser suficientemente óbvios para que isso não prejudique
  //a facilidade de leitura....
  {
    size_t a, count;
    Type b = 1.0;
    if (len)
      {
        if (rest)
          {
            if (len > 1)
              {
                *rest = &s[1];
              }
            else
              {
                *rest = nullptr;
              }
          }
        switch (mode)
          {
          case (SIC_PREFIXES):
            switch (s[0])
              {
              case 'E':
                return 1e18;
              case 'P':
                return 1e15;
              case 'T':
                return 1e12;
              case 'G':
                return 1e9;
              case 'M':
                return 1e6;
              case 'k':
                return 1e3;
              case 'h':
                return 1e2;
              case 'd':
                if (len > 1 && s[1] == 'a')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 10.0;
                  }
                else
                  {
                    return 0.1;
                  }
                break;
              case 'c':
                return 1e-2;
              case 'm':
                return 1e-3;
              case BEGINMU1:
                if (len < 2 || s[1] != ENDMU1)
                  {
                    break;
                  }
                if (rest)
                  {
                    if (len > 2)
                      {
                        *rest = &s[2];
                      }
                    else
                      {
                        *rest = nullptr;
                      }
                  }
                return 1e-6;
              case BEGINMU2:
                if (len < 2 || s[1] != ENDMU2)
                  {
                    break;
                  }
                if (rest)
                  {
                    if (len > 2)
                      {
                        *rest = &s[2];
                      }
                    else
                      {
                        *rest = nullptr;
                      }
                  }
                return 1e-6;
              case 'u':
                return 1e-6;
              case 'n':
                return 1e-9;
              case 'p':
                return 1e-12;
              case 'f':
                return 1e-15;
              case 'a':
                return 1e-18;
              case 'z':
                return 1e-21;
              case 'y':
                return 1e-24;
              default:
                if(rest)
                  {
                    if (len > 1)
                      {
                        *rest = &s[1];
                      }
                    else
                      {
                        *rest = nullptr;
                      }
                  }
                break;
              }
            break;
          case (SIC_LENGTH):
            switch(s[0])
              {
              case 'm':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    break;
                  }
                else if (len > 1 && s[1] == 'i')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 1609.344;
                  }
                else
                  {
                    return 1e-3 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
              case 'f':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    return 1e-15;
                  }
                else if (len > 1 && s[1] == 't')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 0.3048;
                  }
                else
                  {
                    return 1e-15 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
              case 'i':
                if (len > 1 && s[1] == 'n')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 0.0254;
                  }
                else
                  {
                    *rest = s;
                    break;
                  }
              case 'l':
                if (len > 1 && s[1] == 'y')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 9460730472580800.0;
                  }
                else
                  {
                    if (rest)
                      {
                        *rest = s;
                      }
                  }
                break;
              case 'a':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    return 1e-18;
                  }
                else if (len > 1 && (s[1] == 'u' || s[1] == 'U'))
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 149597870700.0;
                  }
                else
                  {
                    return 1e-18 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
                break;
              case 'A':
                if (len > 1 && (s[1] == 'u' || s[1] == 'U'))
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 149597870700.0;
                  }
                else
                  {
                    if (rest)
                      {
                        *rest = s;
                      }
                    break;
                  }
              case 'p':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    return 1e-12;
                  }
                else if (len > 1 && s[1] == 'c')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 3.085677581e16;
                  }
                else
                  {
                    return 1e-12 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
              case 'y':
                if (len == 1)
                  {
                    return 1e-24;
                  }
                else if (len > 1 && s[1] == 'd')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 0.9144;
                  }
                else
                  {
                    return 1e-24 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
              case BEGINANGS1:
                if (len < 2 || s[1] != ENDANGS1)
                  {
                    break;
                  }
                if (rest)
                  {
                    if (len > 2)
                      {
                        *rest = &s[2];
                      }
                    else
                      {
                        *rest = nullptr;
                      }
                  }
                return 1e-10;
              case BEGINANGS2:
                if (len < 3 || s[1] != MIDDLEANGS2 || s[2] != ENDANGS2)
                  {
                    break;
                  }
                if (rest)
                  {
                    if (len > 3)
                      {
                        *rest = &s[3];
                      }
                    else
                      {
                        *rest = nullptr;
                      }
                  }
                return 1e-10;
              default:
                if (rest)
                  {
                    b = si_conversion<Type>(s, SIC_PREFIXES, len, rest);
                    return b * si_conversion<Type>(*rest, SIC_LENGTH, (*rest ? std::strlen(*rest) : 0), rest);
                  }
                break;
              }
            break;
          case (SIC_TIME):
            switch(s[0])
              {
              case 's':
                break;
              case 'h':
                return 3600.0;
              case 'd':
                return 86400.0;
              case 'w':
                return 604800.0;
              case 'a':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    return 1e-18;
                  }
              case 'A':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    return 31557600.0;
                  }
                else if (s[1] == 'n' || s[1] == 'N')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 31557600.0;
                  }
                else
                  {
                    return 1e-18 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
              case 'm':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    return 1e-3;
                  }
                else if(len == 2)
                  {
                    if(s[1] == 'o')
                      {
                        return 2592000.0;
                      }
                    return 1e-3 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
                else
                  {
                    if (s[1] != 'i' || s[2] != 'n')
                      {
                        return 1e-3 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                      }
                    if (rest)
                      {
                        if (len > 3)
                          {
                            *rest = &s[3];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 60;
                  }

              default:
                if (rest)
                  {
                    b = si_conversion<Type>(s, SIC_PREFIXES, len, rest);
                    return b * si_conversion<Type>(*rest, SIC_TIME, (*rest ? std::strlen(*rest) : 0), rest);
                  }
                break;
              }
            break;
          case (SIC_MASS):
            switch(s[0])
              {
              case 'g':
                return 1e-3;
              case 't':
                return 1e3;
              case 'l':
                if (len > 1 && s[1] == 'b')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 0.45359237;
                  }
                else
                  {
                    *rest = s;
                    break;
                  }
              case 'o':
                if (len > 1 && s[1] == 'z')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 2.8349523125e-3;
                  }
                else
                  {
                    if (rest)
                      {
                        *rest = s;
                      }
                    break;
                  }
              case 's':
                if (len > 1 && s[1] == 't')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 6.35029318;
                  }
                else if(len > 3 && s[1] == 'l' && s[2] == 'u' && s[3] == 'g')
                  {
                    if (rest)
                      {
                        if (len > 4)
                          {
                            *rest = &s[4];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 14.59390294;
                  }
                else
                  {
                    if (rest)
                      {
                        *rest = s;
                      }
                    break;
                  }
              default:
                if (rest)
                  {
                    b = si_conversion<Type>(s, SIC_PREFIXES, len, rest);
                    return b * si_conversion<Type>(*rest, SIC_MASS, (*rest ? std::strlen(*rest) : 0), rest);
                  }
                break;
              }
            break;
          case (SIC_ELECTRIC_CHARGE):
            switch(s[0])
              {
              case 'C':
                break;
              case 'a':
                if (len == 1 || !std::isalpha(s[1]))
                  {
                    *rest = s;
                    break;
                  }
                else if (len > 2 && s[1] == 'b' && s[2] == 'C')
                  {
                    if (rest)
                      {
                        if (len > 3)
                          {
                            *rest = &s[3];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 10.0;
                  }
                else
                  {
                    return 1e-18 * si_conversion<Type>(&s[1], SIC_LENGTH, len - 1, rest);
                  }
              case 's':
                if (len > 4 && s[1] == 't' && s[2] == 'a' && s[3] == 't' && s[4] == 'C')
                  {
                    if (rest)
                      {
                        if (len > 5)
                          {
                            *rest = &s[5];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return 3.335640951982e-10;
                  }
                else
                  {
                    *rest = s;
                    break;
                  }
              case 'q':
                if (len > 1 && s[1] == 'e')
                  {
                    if (rest)
                      {
                        if (len > 2)
                          {
                            *rest = &s[2];
                          }
                        else
                          {
                            *rest = nullptr;
                          }
                      }
                    return QE__;
                  }
                else
                  {
                    *rest = s;
                    break;
                  }
                break;
              default:
                if (rest)
                  {
                    b = si_conversion<Type>(s, SIC_PREFIXES, len, rest);
                    return b * si_conversion<Type>(*rest, SIC_ELECTRIC_CHARGE, (*rest ? std::strlen(*rest) : 0), rest);
                  }
                break;
              }
          case (SIC_VELOCITY):
            if (rest)
              {
                for (a = 0; s[a] != 0 && a < len; ++a)
                  {
                    if (s[a] == '/' || s[a] == '\\' || s[a] == '|')
                      {
                        b = si_conversion<Type>(s, SIC_LENGTH, a, rest);
                        return b/si_conversion<Type>(&s[a+1], SIC_TIME, len - a - 1, rest);
                      }
                  }
              }
            break;
          case (SIC_ACCELERATION):
            if (rest)
              {
                count = 0;
                for (a = 0; s[a] != 0 && a < len; ++a)
                  {
                    if (s[a] == '/' || s[a] == '\\' || s[a] == '|')
                      {
                        ++count;
                      }
                    if (count > 1)
                      {
                        b = si_conversion<Type>(s, SIC_VELOCITY, a, rest);
                        return b/si_conversion<Type>(&s[a+1], SIC_TIME, len - a - 1, rest);
                      }
                  }
              }
            break;
          case (SIC_LINEAR_MOMENTUM):
            if (rest)
              {
                b = si_conversion<Type>(s, SIC_MASS, len, rest);
                return b * si_conversion<Type>(*rest, SIC_VELOCITY, len, rest);
              }
            break;
          default:
            break;
          }
      }
    return 1.0;
  }


  template <class Type> Type parse_string(const char* str, siconvertmode mode)
  {
    Type num;
    size_t i = 0, j = 0;
    char *t = nullptr, *u;
    num = (Type) std::strtod(str, &u);
    if (u)
      {
        for (i = 0; u[i] != 0; ++i)
          {
            if (std::iscntrl(u[i]) || std::ispunct(u[i]) || std::isspace(u[i]))
              {
                continue;
              }
            if (!t)
              {
                j = i;
                t = &u[i];
              }
          }
        i -= j;
      }
    return num * si_conversion<Type>(t, mode, i, &u);
  }

#undef QE__
#undef BEGINMU1
#undef ENDMU1
#undef BEGINMU2
#undef ENDMU2
#undef BEGINANGS1
#undef ENDANGS1
#undef BEGINANGS2
#undef MIDDLEANGS2
#undef ENDANGS2

}
//Nota: pela complexidade das classes e funções criadas,
//optou-se por incluir directamente o código nos programas
//a compilar, por oposição a usar o header com o código à parte.
//É certo que se perde a vantagem da compilação separada,
//aumentando, portanto, o tempo de compilação,
//mas considerou-se que a maior facilidade na implementação
//de novos métodos nestas classes, conforme as necessidades,
//supera essas desvantagens.

#endif
