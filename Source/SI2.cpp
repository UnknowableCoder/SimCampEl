#ifndef SII_SIUInt
#define SII_SIUInt

/**************************************
 *                                    *
 *    SI^2: SI (Units) Interpreter    *
 *                                    *
 * Main Coder: Nuno Fernandes         *
 * Aux. Coder & Tester: Gonçalo Vaz   *
 *                                    *
 **************************************/
//Based on:
//https://www.nist.gov/sites/default/files/documents/2016/12/07/sp330.pdf


#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>
#include <sstream>
#include <cstdint>
#include <vector>

namespace SII
{

  template <class base_T, class exp_T = size_t> base_T fastpow(const base_T &base, exp_T exp)
  //Efficient implementation of exponentiation by integers.
  //Since SI units always appear with integer exponents,
  //this is all that is needed...
  //exp_T should be an unsigned integer type.
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

  template <class Dest, class Str, class Size> inline void update_stringlike(Dest & dest, const Str & orig, const Size & size)
  {
    Size i;
    for (i = 0; i < size; ++i)
      {
        dest += orig[i];
      }
  }

  template <class Rep = std::string, class Hold = int> class SI_type
  //Rep is the class which should hold the string representation of the units.
  //It must have an overload for operator+= which enables it to concatenate
  //chars (or any other type to which char may be converted), as well as operator[]
  //to access sucessive members of the array.
  //
  //Hold is the class that should hold the dimensionality information;
  //it should be a signed integral type, or equivalent.
  {
  public:
    Hold length, time, mass, temperature, current,
      intensity, amount;
#if USE_RADIANS
    Hold radians, steradians;
#endif
    bool becquerel;
    bool gray;
#if USE_RADIANS
    SI_type(const Hold a = 0, const Hold b = 0, const Hold c = 0, const Hold d = 0, const Hold e = 0, const Hold f = 0, const Hold g = 0, const Hold h = 0, const Hold i = 0, const bool j = false, const bool k = false)
#else
      SI_type(const Hold a = 0, const Hold b = 0, const Hold c = 0, const Hold d = 0, const Hold e = 0, const Hold f = 0, const Hold g = 0, const bool j = false, const bool k = false)
#endif
    {
      length = a;
      time = b;
      mass = c;
      temperature = d;
      current = e;
      intensity = f;
      amount = g;
#if USE_RADIANS
      radians = h;
      steradians = i;
#endif
      becquerel = j;
      gray = k;
    }
    SI_type (const SI_type& t)
    {
      length = t.length;
      time = t.time;
      mass = t.mass;
      temperature = t.temperature;
      current = t.current;
      intensity = t.intensity;
      amount = t.amount;
#if USE_RADIANS
      radians = t.radians;
      steradians = t.steradians;
#endif
      becquerel = t.becquerel;
      gray = t.gray;
    }
#if USE_RADIANS
    void set(const Hold a = 0, const Hold b = 0, const Hold c = 0, const Hold d = 0, const Hold e = 0, const Hold f = 0, const Hold g = 0, const Hold h = 0, const Hold i = 0, const bool j = false, const bool k = false)
#else
      void set(const Hold a = 0, const Hold b = 0, const Hold c = 0, const Hold d = 0, const Hold e = 0, const Hold f = 0, const Hold g = 0, const bool j = false, const bool k = false)
#endif
    {
      length = a;
      time = b;
      mass = c;
      temperature = d;
      current = e;
      intensity = f;
      amount = g;
#if USE_RADIANS
      radians = h;
      steradians = i;
#endif
      becquerel = j;
      gray = k;
    }
    Hold& operator[] (const int i)
    {
      switch (i)
        {
        default:
        case 0:
          return length;
        case 1:
          return time;
        case 2:
          return mass;
        case 3:
          return temperature;
        case 4:
          return current;
        case 5:
          return intensity;
        case 6:
          return amount;
#if USE_RADIANS
        case 7:
          return radians;
        case 8:
          return steradians;
#endif
        }
    }
    Hold operator[] (const int i) const
    {
      switch (i)
        {
        default:
        case 0:
          return length;
        case 1:
          return time;
        case 2:
          return mass;
        case 3:
          return temperature;
        case 4:
          return current;
        case 5:
          return intensity;
        case 6:
          return amount;
#if USE_RADIANS
        case 7:
          return radians;
        case 8:
          return steradians;
#endif
        }
    }
  private:

#ifdef SII_LOCAL_get_text
#error For SII to work properly, the macro 'SII_LOCAL_get_text' musn't be defined. Please change your macro's name and/or the order in which you include SII's code (the macro doesn't remain defined after the code, so the name can still be used).
#endif

#define SII_LOCAL_get_text(VAR, STRING, SIZE)   \
    {                                           \
      if(ret.size())                            \
        {                                       \
          ret += ' ';                           \
        }                                       \
      update_stringlike(ret, STRING, SIZE);     \
      if(VAR != 1)                              \
        {                                       \
          ret += '^';                           \
          ret += '{';                           \
          if(VAR < 0)                           \
            {                                   \
              ret += '-';                       \
            }                                   \
          int i = abs(VAR);                     \
          Rep temp;                             \
          while (i > 0)                         \
            {                                   \
              temp += ('0' + i % 10);           \
              i /= 10;                          \
            }                                   \
          i = temp.size();                      \
          while(i >= 0)                         \
            {                                   \
              ret += temp[i];                   \
              --i;                              \
            }                                   \
          ret += '}';                           \
        }                                       \
    }
    //Just to abbreviate the code...

    Rep process_full_units() const
    {
      Rep ret;
      if (length > 0)
        SII_LOCAL_get_text(length,"m", 1);
      if (mass > 0)
        SII_LOCAL_get_text(mass, "kg", 2);
      if(becquerel && time < 0)
        {
          SII_LOCAL_get_text(-time, "Bq", 2);
        }
      else if (time > 0)
        {
          SII_LOCAL_get_text(time, "s", 1);
        }
      if(current > 0)
        SII_LOCAL_get_text(current, "A", 1);
      if(amount > 0)
        SII_LOCAL_get_text(amount, "mol", 3);
      if(temperature > 0)
        SII_LOCAL_get_text(temperature, "K", 1);
      if(intensity > 0)
        SII_LOCAL_get_text(intensity, "cd", 2);
#if USE_RADIANS
      if(radians > 0)
        SII_LOCAL_get_text(radians, "rad", 3);
      if (steradians > 0)
        SII_LOCAL_get_text(steradians, "sr", 2);
#endif

      if (length < 0)
        SII_LOCAL_get_text(length,"m", 1);
      if (mass < 0)
        SII_LOCAL_get_text(mass, "kg", 2);
      if(becquerel && time > 0)
        {
          SII_LOCAL_get_text(-time, "Bq", 2);
        }
      else if (time < 0)
        {
          SII_LOCAL_get_text(time, "s", 1);
        }
      if(current < 0)
        SII_LOCAL_get_text(current, "A", 1);
      if(amount < 0)
        SII_LOCAL_get_text(amount, "mol", 3);
      if(temperature < 0)
        SII_LOCAL_get_text(temperature, "K", 1);
      if(intensity < 0)
        SII_LOCAL_get_text(intensity, "cd", 2);
#if USE_RADIANS
      if(radians < 0)
        SII_LOCAL_get_text(radians, "rad", 3);
      if (steradians < 0)
        SII_LOCAL_get_text(steradians, "sr", 2);
#endif
      //Repeated to allow for constructions like "kg m^{-3}",
      //instead of "m^{-3} kg"...

      return ret;
    }

#undef SII_LOCAL_get_text

  public:
#if USE_RADIANS
    bool check_dimensions (const Hold a = 0, const Hold b = 0, const Hold c = 0, const Hold d = 0, const Hold e = 0, const Hold f = 0, const Hold g = 0, const Hold h = 0, const Hold i = 0, const bool j = false, const bool k = false) const
    {
      return (length == a) && (time == b) &&
        (mass == c) && (temperature == d) &&
        (current == e) && (intensity == f) &&
        (amount == g) && (radians == h) &&
        (steradians == i) && (becquerel == j) && (gray == k);
    }
#else
    bool check_dimensions (const Hold length_ = 0, const Hold time_ = 0,
                           const Hold mass_ = 0, const Hold temperature_ = 0,
                           const Hold current_ = 0, const Hold intensity_ = 0,
                           const Hold amount_ = 0, const bool becquerel_ = false,
                           const bool gray_ = false) const
    {
      return (length == length_) && (time == time_) &&
        (mass == mass_) && (temperature == temperature_) &&
        (current == current_) && (intensity == intensity_) &&
        (amount == amount_) && (becquerel == becquerel_) && (gray == gray_);
    }
#endif

    Rep get_SI_units(const bool all_units = false) const
    {
      Rep ret;
      if(becquerel || all_units)
        {
          return process_full_units();
        }
      if(check_dimensions(0,-1)) //Frequency
        {
          ret += 'H';
          ret += 'z';
          return ret;
        }
      if(check_dimensions(1,-2,1)) //Force
        {
          ret += 'N';
          return ret;
        }
      if(check_dimensions(-1,-2,1))//Pressure;
        {
          ret += 'P';
          ret += 'a';
          return ret;
        }
      if(check_dimensions(2,-2,1))//Energy
        {
          ret += 'J';
          return ret;
        }
      if(check_dimensions(2,-3,1)) //Power
        {
          ret += 'W';
          return ret;
        }
      if(check_dimensions(0,1,0,0,1)) //Charge
        {
          ret += 'C';
          return ret;
        }
      if(check_dimensions(2,-3,1,0,-1)) //Voltage
        {
          ret += 'V';
          return ret;
        }
      if(check_dimensions(-2,4,-1,0,2))//Capacitance
        {
          ret += 'F';
          return ret;
        }
      if(check_dimensions(2,-3,1,0,-2)) //Resistance
        {
          ret += '\\';
          ret += 'O';
          ret += 'm';
          ret += 'e';
          ret += 'g';
          ret += 'a';
          return ret;
        }
      //(For the sake of simplicity and portability,
      //the Omega symbol is rendered as if
      //in LaTeX source code, much like the exponents.)
      if(check_dimensions(-2,3,-1,0,2)) //Conductance
        {
          ret += 'S';
          return ret;
        }
      if(check_dimensions(2,-2,1,0,-1)) //Magnetic flux
        {
          ret += 'W';
          ret += 'b';
          return ret;
        }
      if(check_dimensions(0,-2,1,0,-1)) //Magnetic flux density
        {
          ret += 'T';
          return ret;
        }
      if(check_dimensions(2,-2,1,0,-2)) //Inductance
        {
          ret += 'H';
          return ret;
        }
#if USE_RADIANS
      if(check_dimensions(0,0,0,0,0,1,0,0,1)) //Luminous flux
        {
          ret += 'l';
          ret += 'm';
          return ret;
        }
      if(check_dimensions(-2,0,0,0,0,1,0,0,1))//Illuminance
        {
          ret += 'l';
          ret += 'x';
          return ret;
        }
#else
      if(check_dimensions(0,0,0,0,0,1)) //Luminous flux
        {
          ret += 'l';
          ret += 'm';
          return ret;
        }
      if(check_dimensions(-2,0,0,0,0,1))//Illuminance
        {
          ret += 'l';
          ret += 'x';
          return ret;
        }
#endif
      if(check_dimensions(2,-2) && gray) //Absorbed dose
        {
          ret += 'G';
          ret += 'y';
          return ret;
        }
      if(check_dimensions(2,-2) && !gray)//Dose equivalent
        {
          ret += 'S';
          ret += 'v';
        }
      if(check_dimensions(0,-1,0,0,0,0,1))//Catalytic activity
        {
          ret += 'k';
          ret += 'a';
          ret += 't';
          return ret;
        }
      if(check_dimensions(-1,-1,1)) //Dynamic viscosity
        {
          ret += 'P';
          ret += 'a';
          ret += ' ';
          ret += 's';
          return ret;
        }
      if(check_dimensions(2,-2,1)) //Moment of force (torque?)
        {
          ret += 'N';
          ret += ' ';
          ret += 'm';
          return ret;
        }
      if(check_dimensions(0,-2,1)) //Surface tension
        {
          ret += 'N';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(0,-3,1)) //Irradiance/Heat flux density
        {
          ret += 'W';
          ret += ' ';
          ret += 's';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '2';
          ret += '}';
          return ret;
        }
      if(check_dimensions(2,-2,1,-1)) //Heat capacity
        {
          ret += 'J';
          ret += ' ';
          ret += 'K';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(2,-2,0,-1)) //Specific heat capacity
        {
          ret += 'J';
          ret += ' ';
          ret += 'k';
          ret += 'g';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          ret += ' ';
          ret += 'K';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(2,-2)) //Specific energy
        {
          ret += 'J';
          ret += ' ';
          ret += 'k';
          ret += 'g';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(1,-3,1,-1)) //Thermal conductivity
        {
          ret += 'W';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          ret += ' ';
          ret += 'K';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(-1,-2,1)) //Energy density
        {
          ret += 'J';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '3';
          ret += '}';
          return ret;
        }
      if(check_dimensions(1,-3,1,0,-1))//Electric field
        {
          ret += 'V';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(-3,1,0,0,1)) //Charge density
        {
          ret += 'C';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '3';
          ret += '}';
          return ret;
        }
      if(check_dimensions(-2,1,0,0,1)) //Surface charge density et alii
        {
          ret += 'C';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '2';
          ret += '}';
          return ret;
        }
      if(check_dimensions(-3,4,-1,0,2)) //Permmittivity
        {
          ret += 'F';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(1,-2,1,0,-2)) //Permeability
        {
          ret += 'H';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(2,-2,1,0,0,0,-1)) //Molar energy
        {
          ret += 'J';
          ret += ' ';
          ret += 'm';
          ret += 'o';
          ret += 'l';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(2,-2,1,-1,0,0,-1))//Molar heat capacity
        {
          ret += 'J';
          ret += ' ';
          ret += 'm';
          ret += 'o';
          ret += 'l';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          ret += ' ';
          ret += 'K';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(0,1,-1,0,1)) //Exposure (radiation)
        {
          ret += 'C';
          ret += ' ';
          ret += 'k';
          ret += 'g';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(2,-3) && gray)//Absorbed dose rate
        {
          ret += 'G';
          ret += 'y';
          ret += ' ';
          ret += 's';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
#if USE_RADIANS
      if(check_dimensions(2,-3,1,0,0,0,0,0,-1)) //Radiant intensity
        {
          ret += 'W';
          ret += ' ';
          ret += 's';
          ret += 'r';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
      if(check_dimensions(0,-3,1,0,0,0,0,0,-1)) //Radiance
        {
          ret += 'W';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '2';
          ret += '}';
          ret += ' ';
          ret += 's';
          ret += 'r';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '1';
          ret += '}';
          return ret;
        }
#else
      if(check_dimensions(0,-3,1,0,0,0,0,0,-1)) //Radiance
        {
          ret += 'W';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '2';
          ret += '}';
          return ret;
        }
#endif
      if(check_dimensions(-3,-1,0,0,0,0,1)) //Catalytic activity concentration
        {
          ret += 'k';
          ret += 'a';
          ret += 't';
          ret += ' ';
          ret += 'm';
          ret += '^';
          ret += '{';
          ret += '-';
          ret += '3';
          ret += '}';
          return ret;
        }

      return process_full_units();
    }

    SI_type operator+ (const SI_type &t) const
    {
      SI_type temp(t);
      temp.length += length;
      temp.time += time;
      temp.mass += mass;
      temp.temperature += temperature;
      temp.current += current;
      temp.intensity += intensity;
      temp.amount += amount;
#if USE_RADIANS
      temp.radians += radians;
      temp.steradians += steradians;
#endif
      temp.becquerel = becquerel || temp.becquerel;
      //Combining Hz and Bq seems to be most unusual,
      //so this is essentially arbitrary.
      temp.gray = gray || temp.gray;
      //The same thing for Gy and Sv...
      return temp;
    }
    SI_type operator- (const SI_type &t) const
    {
      SI_type temp(t);
      temp.length -= length;
      temp.time -= time;
      temp.mass -= mass;
      temp.temperature -= temperature;
      temp.current -= current;
      temp.intensity -= intensity;
      temp.amount -= amount;
#if USE_RADIANS
      temp.radians -= radians;
      temp.steradians -= steradians;
#endif
      temp.becquerel = becquerel || temp.becquerel;
      temp.gray = gray || temp.gray;
      return temp;
    }

    SI_type operator- () const
    {
      SI_type temp();
      temp.length = -temp.length;
      temp.time = -temp.time;
      temp.mass = -temp. mass;
      temp.temperature = -temp.temperature;
      temp.current = -temp.current;
      temp.intensity = -temp.intensity;
      temp.amount = -temp.amount;
#if USE_RADIANS
      temp.radians = -temp.radians;
      temp.steradians = -temp.steradians;
#endif
      return temp;
      //Gray and Becquerel shouldn't be altered by this...
    }

    SI_type operator* (const Hold i) const
    {
      SI_type temp(*this);
      temp.length *= i;
      temp.time *= i;
      temp.mass *= i;
      temp.temperature *= i;
      temp.current *= i;
      temp.intensity *= i;
      temp.amount *= i;
#if USE_RADIANS
      temp.radians *= i;
      temp.steradians *= i;
#endif
      return temp;
    }

    SI_type operator/ (const Hold i) const
    {
      SI_type temp(*this);
      temp.length /= i;
      temp.time /= i;
      temp.mass /= i;
      temp.temperature /= i;
      temp.current /= i;
      temp.intensity /= i;
      temp.amount /= i;
#if USE_RADIANS
      temp.radians /= i;
      temp.steradians /= i;
#endif
      return temp;
    }

    SI_type& operator*= (const Hold i)
    {
      length *= i;
      time *= i;
      mass *= i;
      temperature *= i;
      current *= i;
      intensity *= i;
      amount *= i;
#if USE_RADIANS
      radians *= i;
      steradians *= i;
#endif
      return (*this);
    }

    SI_type& operator/= (const Hold i)
    {
      length /= i;
      time /= i;
      mass /= i;
      temperature /= i;
      current /= i;
      intensity /= i;
      amount /= i;
#if USE_RADIANS
      radians /= i;
      steradians /= i;
#endif
      return (*this);
    }
    SI_type& operator+= (const SI_type &t)
    {
      length += t.length;
      time += t.time;
      mass += t.mass;
      temperature += t.temperature;
      current += t.current;
      intensity += t.intensity;
      amount += t.amount;
#if USE_RADIANS
      radians += t.radians;
      steradians += t.steradians;
#endif
      becquerel = becquerel || t.becquerel;
      gray = gray || t.gray;
      return (*this);
    }
    SI_type& operator-= (const SI_type &t)
    {
      length -= t.length;
      time -= t.time;
      mass -= t.mass;
      temperature -= t.temperature;
      current -= t.current;
      intensity -= t.intensity;
      amount -= t.amount;
#if USE_RADIANS
      radians -= t.radians;
      steradians -= t.steradians;
#endif
      becquerel = becquerel || t.becquerel;
      gray = gray || t.gray;
      return (*this);
    }

    SI_type& operator= (const SI_type &t)
    {
      if(&t != this)
        {
          length = t.length;
          time = t.time;
          mass = t.mass;
          temperature = t.temperature;
          current = t.current;
          intensity = t.intensity;
          amount = t.amount;
#if USE_RADIANS
          radians = t.radians;
          steradians = t.steradians;
#endif
          becquerel = t.becquerel;
          gray = t.gray;
        }
      return (*this);
    }
    bool operator== (const SI_type &t) const
    {
#if USE_RADIANS
      return t.check_dimensions(length,time,mass,temperature,current,intensity,amount,radians,steradians,becquerel,gray);
#else
      return t.check_dimensions(length,time,mass,temperature,current,intensity,amount,becquerel,gray);
#endif
    }
    bool operator!= (const SI_type &t) const
    {
      return !(*this == t);
    }
    void reset()
    {
      length = 0;
      time = 0;
      mass = 0;
      temperature = 0;
      current = 0;
      intensity = 0;
      amount = 0;
#if USE_RADIANS
      radians = 0;
      steradians = 0;
#endif
      becquerel = false;
      gray = false;
    }
  };

  template <class any, class Rep = std::string, class Hold = int> std::basic_ostream<any>& operator<< (std::basic_ostream<any> &s, const SI_type<Rep, Hold> &t)
  //The ability to print a SI_type serves essentially for debugging purposes.
  //It would be infinitely more useful to print the return of its get_SI_units(true)
  //if one wants to analyse its true dimensional properties...
  {
#if USE_RADIANS
    s << t.length << " " << t.time << " " << t.mass
      << " " << t.temperature << " " << t.current
      << " " << t.intensity << " " << t.amount
      << " " << t.radians << " " << t.steradians
      << " " << t.becquerel << " " << t.gray;
#else
    s << t.length << " " << t.time << " " << t.mass
      << " " << t.temperature << " " << t.current
      << " " << t.intensity << " " << t.amount
      << " " << t.becquerel << " " << t.gray;
#endif
    return s;
  }

  template <class Num, class Str = std::string, class Hold = int> struct SI_value
  //Num holds the numerical value, Str dictates the string representation
  //used by the SI_type that should hold the dimensions.
  {
  public:
    Num value;
    SI_type<Str, Hold> dimensions;
#if USE_RADIANS
    SI_value():
      value(1), dimensions(0,0,0,0,0,0,0,0,0,false,false) {}
    //The default must be 1 for multiplication neutrality
    //in the event that a prefix is not recognized...
#else
    SI_value():
      value(1), dimensions(0,0,0,0,0,0,0,false,false) {}
    //The default must be 1 for multiplication neutrality
    //in the event that a prefix is not recognized...
#endif
    SI_value(const SI_value& t):
      value(t.value), dimensions(t.dimensions) {}
#if USE_RADIANS
    SI_value(const Num& v):
      value(v), dimensions(0,0,0,0,0,0,0,0,0,false,false) {}
#else
    SI_value(const Num& v):
      value(v), dimensions(0,0,0,0,0,0,0,false,false) {}
#endif
    SI_value(const Num& v, const SI_type<Str, Hold> &d):
      value(v), dimensions(d) {}
    operator Num() const
    {
      return value;
    }
    Str get_SI_units(const bool all_units = false) const
    {
      return dimensions.get_SI_units(all_units);
    }
    SI_value operator+ (const SI_value& v) const
    {
      if(dimensions != v.dimensions)
        {
          std::cerr << "Error! Dimensional mismatch between (" << dimensions << ") and (" << v.dimensions << ")!" << std::endl;
          return (*this);
        }
      SI_value temp(*this);
      temp.value += v.value;
      return temp;
    }
    SI_value operator+ (const Num& n) const
    {
      SI_value temp(*this);
      temp.value += n;
      return temp;
    }
    SI_value operator- (const SI_value& v) const
    {
      if(dimensions != v.dimensions)
        {
          std::cerr << "Error! Dimensional mismatch between (" << dimensions << ") and (" << v.dimensions << ")!" << std::endl;
          return (*this);
        }
      SI_value temp(*this);
      temp.value -= v.value;
      return temp;
    }
    SI_value operator- (const Num& n) const
    {
      SI_value temp(*this);
      temp.value -= n;
      return temp;
    }
    SI_value operator* (const SI_value& v) const
    {
      SI_value temp(*this);
      temp.value *= v.value;
      temp.dimensions += v.dimensions;
      return temp;
    }
    SI_value operator/ (const SI_value& v) const
    {
      SI_value temp(*this);
      temp.value /= v.value;
      temp.dimensions -= v.dimensions;
      return temp;
    }
    SI_value operator* (const Num& n) const
    {
      SI_value temp(*this);
      temp.value *= n;
      return temp;
    }
    SI_value operator/ (const Num& n) const
    {
      SI_value temp(*this);
      temp.value /= n;
      return temp;
    }
    SI_value operator- () const
    {
      SI_value temp(*this);
      temp.value = -temp.value;
      return temp;
    }
    bool operator== (const SI_value &v) const
    {
      return (value == v.value) && (dimensions == v.dimensions);
    }
    bool operator!= (const SI_value &v) const
    {
      return (value != v.value) || (dimensions != v.dimensions);
    }
    SI_value& operator = (const SI_value &v)
    {
      if (&v != this)
        {
          value = v.value;
          dimensions = v.dimensions;
        }
      return (*this);
    }
    SI_value& operator += (const SI_value &v)
    {
      if(dimensions != v.dimensions)
        {
          std::cerr << "Error! Dimensional mismatch between (" << dimensions << ") and (" << v.dimensions << ")!" << std::endl;
          return (*this);
        }
      value += v.value;
      return (*this);
    }
    SI_value& operator -= (const SI_value &v)
    {
      if(dimensions != v.dimensions)
        {
          std::cerr << "Error! Dimensional mismatch between (" << dimensions << ") and (" << v.dimensions << ")!" << std::endl;
          return (*this);
        }
      value -= v.value;
      return (*this);
    }
    SI_value& operator *= (const SI_value &v)
    {
      value *= v.value;
      dimensions += v.dimensions;
      return (*this);
    }
    SI_value& operator /= (const SI_value &v)
    {
      value /= v.value;
      dimensions -= v.dimensions;
      return (*this);
    }
    SI_value& operator *= (const Num &n)
    {
      value *= n;
      return (*this);
    }
    SI_value& operator /= (const Num &n)
    {
      value /= n;
      return (*this);
    }
    SI_value& operator += (const Num &n)
    {
      value += n;
      return (*this);
    }
    SI_value& operator -= (const Num &n)
    {
      value -= n;
      return (*this);
    }
    void reset()
    {
      value = 1;
      dimensions.reset();
    }
  };


  template <class any, class v, class Rep = std::string, class Hold = int> std::basic_ostream<any>& operator<< (std::basic_ostream<any> &s, const SI_value<v, Rep, Hold> &t)
  //The ability to print a SI_type serves essentially for debugging purposes.
  //It would be infinitely more useful to print the return of its get_SI_units(true)
  //if one wants to analyse its true dimensional properties...
  {
    s << t.value << " " << t.get_SI_units();
    return s;
  }

  template <class stringtype = std::string> size_t get_with_par(std::basic_istream<typename stringtype::value_type> &is, stringtype &str, const typename stringtype::value_type delim = '\n', const bool append = true)
  //If append is true, then the extracted characters are appended to str;
  //if it is false, then it behaves like getline() and the extracted characters
  //replace the contents of the string.
  {
    if(!append)
      {
        str.clear();
      }
    typename stringtype::value_type c;
    while(is.good())
      {
        c = is.get();
        if(!is.good())
          {
            return 1;
          }
        if(c == delim)
          {
            return 0;
          }
        switch(c)
          {
          case '(':
            str += c;
            get_with_par<stringtype>(is, str, ')', true);
            str += ')'; //The delimiter is discarded by get_with_par...
            break;
          case '[':
            str += c;
            get_with_par<stringtype>(is, str, ']', true);
            str += ']'; //The delimiter is discarded by get_with_par...
            break;
          case '{':
            str += c;
            get_with_par<stringtype>(is, str, '}', true);
            str += '}'; //The delimiter is discarded by get_with_par...
            break;
          case '|':
            str += c;
            get_with_par<stringtype>(is, str, c, true);
            str += c; //The delimiter is discarded...
            break;
          case '\0':
            return 1;
            break;
          default:
            str += c;
            break;
          }
      }
    return 1;
  }

  template <class Type> size_t vectorcontains(const std::vector<Type> &v, const Type &e)
  {
    size_t i;
    for(i = 0; i < v.size(); ++i)
      {
        if(v[i] == e)
          {
            return true;
          }
      }
    return false;
  }

  template <class Type> size_t vectorcontains(const std::vector<Type> &v, const Type &e, size_t &i)
  {
    for(i = 0; i < v.size(); ++i)
      {
        if(v[i] == e)
          {
            return true;
          }
      }
    return false;
  }


  template <class stringtype = std::string> size_t get_with_par(std::basic_istream<typename stringtype::value_type> &is, stringtype &str, const std::vector<typename stringtype::value_type> &delim, const bool append = true)
  //Like before, but now allows for several delims...
  //It returns which delim was found...
  {
    if(!append)
      {
        str.clear();
      }
    typename stringtype::value_type c;
    size_t num;
    while(is.good())
      {
        c = is.get();
        if(!is.good())
          {
            return delim.size();
          }
        if(vectorcontains(delim, c, num))
          {
            return num;
          }
        switch(c)
          {
          case '(':
            str += c;
            get_with_par<stringtype>(is, str, ')', true);
            str += ')'; //The delimiter is discarded by get_with_par...
            break;
          case '[':
            str += c;
            get_with_par<stringtype>(is, str, ']', true);
            str += ']'; //The delimiter is discarded by get_with_par...
            break;
          case '{':
            str += c;
            get_with_par<stringtype>(is, str, '}', true);
            str += '}'; //The delimiter is discarded by get_with_par...
            break;
          case '|':
            str += c;
            get_with_par<stringtype>(is, str, c, true);
            str += c; //The delimiter is discarded...
            break;
          case '\0':
            return delim.size();
            break;
          default:
            str += c;
            break;
          }
      }
    return delim.size();
  }

  template <class stringtype = std::string> size_t get_with_par(std::basic_istream<typename stringtype::value_type> &is, stringtype &str, const std::vector<stringtype> &delim, const bool append = true)
  //And now with multi-char delims...
  {
    if(!append)
      {
        str.clear();
      }
    typename stringtype::value_type c;
    stringtype temp;
    size_t num, maxsize = 0;
    for (num = 0; num < delim.size(); ++num)
      {
        maxsize = std::max(maxsize, delim[num].size());
      }
    while(is.good())
      {
        c = is.get();
        if(!is.good())
          {
            str += temp;
            return delim.size();
          }
        if(vectorcontains(delim, temp, num))
          {
            return num;
          }
        else if(temp.size() > maxsize)
          {
            str += temp;
            temp.clear();
          }
        switch(c)
          {
          case '(':
            str += c;
            get_with_par<stringtype>(is, str, ')', true);
            str += ')'; //The delimiter is discarded by get_with_par...
            break;
          case '[':
            str += c;
            get_with_par<stringtype>(is, str, ']', true);
            str += ']'; //The delimiter is discarded by get_with_par...
            break;
          case '{':
            str += c;
            get_with_par<stringtype>(is, str, '}', true);
            str += '}'; //The delimiter is discarded by get_with_par...
            break;
          case '|':
            str += c;
            get_with_par<stringtype>(is, str, c, true);
            str += c; //The delimiter is discarded...
            break;
          case '\0':
            str += temp;
            return delim.size();
            break;
          default:
            temp += c;
            break;
          }
      }
    return delim.size();
  }

  template <class Out, class In = std::string, class Hold = int> class SI_interpreter
  //In is the input type, Out is the output type...
  {
  private:
    mutable std::unordered_map<In, SI_value<Out, In, Hold>> assoc;
    //This holds the associations between strings and SI_values...
    mutable std::unordered_map<In, Out> prefix;
    //This holds the (presumably metric) prefixes...
    mutable size_t largest_prefix;
    //This indicates the largest prefix (to facilitate the parsing algorithm)


#ifdef SII_LOCAL_do_assoc
#error For SII to work properly, the macro 'SII_LOCAL_do_assoc' musn't be defined. Please change your macro's name and/or the order in which you include SII's code (the macro doesn't remain defined after the code, so the name can still be used).
#endif

#define SII_LOCAL_do_assoc(STR, SIZE, VALUE, DIMS)      \
    {                                                   \
      key.clear();                                      \
      update_stringlike(key, STR, SIZE);                \
      val.value = VALUE;                                \
      val.dimensions.set DIMS;                          \
      assoc[key] = val;                                 \
    }                                                   \


  public:


    void add_CGS_units() const
    //ESU
    //EMU
    //Gaussian
    {
      In key;
      SI_value<Out, In, Hold> val;

      const double pi = 3.14159265358979323846264338327950288419716939937510;
      //Of course, this might be too much precision, but...

      const double c=29979245800;
      //in CGS, of course.

      //Electric charge
      SII_LOCAL_do_assoc("statC", 5, 1e1/c, (0,1,0,0,1) );
      SII_LOCAL_do_assoc("abC", 3, 1e1, (0,1,0,0,1) );
      SII_LOCAL_do_assoc("Fr", 2, 1e1/c, (0,1,0,0,1) );


      //Electric current
      SII_LOCAL_do_assoc("statA", 5, 1e1/c, (0,0,0,0,1) );
      SII_LOCAL_do_assoc("abA", 3, 1e1, (0,0,0,0,1) );

      //Voltage
      SII_LOCAL_do_assoc("statV", 5, 1e-8*c, (2,-3,1,0,-1) );
      SII_LOCAL_do_assoc("abV", 3, 1e-8, (2,-3,1,0,-1) );

      //Electric Dipole Moment
      SII_LOCAL_do_assoc("D", 1, 1e-19/c, (1,1,0,0,1) );

      //Magnetic B Field
      SII_LOCAL_do_assoc("statT", 5, 1e-4*c, (0,-2,1,0,-1) );
      SII_LOCAL_do_assoc("G", 1, 1e-4, (0,-2,1,0,-1) );

      //No ESU
      SII_LOCAL_do_assoc("Oe", 2, 1e3/4/pi, (-1,0,0,0,1) );

      //Magnetic flux
      SII_LOCAL_do_assoc("Mx", 2, 1e-8, (2,-2,1,0,-1) );

      //Resistance
      SII_LOCAL_do_assoc("ab\\Omega", 8, 1e-9, (2,-3,1,0,-2) );

      //Capacitance
      SII_LOCAL_do_assoc("abF", 3, 1e9, (-2,4,-1,0,2) );

      //Inductance
      SII_LOCAL_do_assoc("abH", 3, 1e-9, (2,-2,1,0,-2) );

    }

    void add_more_units() const
    //These are non-SI units, that not in every possible use of SII
    //need be added to the list of recognized symbols...
    {
      In key;
      SI_value<Out, In, Hold> val;

      const double pi = 3.14159265358979323846264338327950288419716939937510;
      //Of course, this might be too much precision, but...


      SII_LOCAL_do_assoc("pi", 2, pi, (0) );
      SII_LOCAL_do_assoc("Pi", 2, pi, (0) );
      SII_LOCAL_do_assoc("\\pi", 3, pi, (0) );
      SII_LOCAL_do_assoc("M_PI", 4, pi, (0) );
      SII_LOCAL_do_assoc("M_Pi", 4, pi, (0) );
      SII_LOCAL_do_assoc("M_pi", 4, pi, (0) );
      SII_LOCAL_do_assoc("m_pi", 4, pi, (0) );

      SII_LOCAL_do_assoc("min", 3, 60, (0,1) );
      SII_LOCAL_do_assoc("h", 1, 3600, (0,1) );
      SII_LOCAL_do_assoc("d", 1, 86400, (0,1) );
#if USE_RADIANS
      SII_LOCAL_do_assoc("o", 1, pi / 180.0, (0,0,0,0,0,0,0,1) );
      SII_LOCAL_do_assoc("\\deg", 4, pi / 180.0, (0,0,0,0,0,0,0,1) );
      //The degree symbol can be semi-problematic...
      SII_LOCAL_do_assoc("'", 1, pi / 10800.0, (0,0,0,0,0,0,0,1) );
      SII_LOCAL_do_assoc("\"", 1, pi / 648000.0, (0,0,0,0,0,0,0,1) );
      SII_LOCAL_do_assoc("''", 2, pi / 648000.0, (0,0,0,0,0,0,0,1) );
#endif
      SII_LOCAL_do_assoc("a", 1, 100, (2) );

      SII_LOCAL_do_assoc("L", 1, 1e-3, (3) );
      SII_LOCAL_do_assoc("l", 1, 1e-3, (3) );

      SII_LOCAL_do_assoc("t", 1, 1000, (0,0,1) );

      SII_LOCAL_do_assoc("eV", 2, 1.602176487e-19, (2,-2,1) );

      SII_LOCAL_do_assoc("ua", 2, 1.49597870691e11, (1) );
      SII_LOCAL_do_assoc("UA", 2, 1.49597870691e11, (1) );

      SII_LOCAL_do_assoc("pc", 2, 1.49597870691e11*648000/pi, (1) );

      SII_LOCAL_do_assoc("u", 1, 1.660538782e-27, (0,0,1) );
      SII_LOCAL_do_assoc("Da", 2, 1.660538782e-27, (0,0,1) );

      SII_LOCAL_do_assoc("c", 1, 299792458, (1,-1) );
      SII_LOCAL_do_assoc("\\h", 2, 6.626070040e-34 , (2,-1,1) );
      //To avoid ambiguity with hour.
#if USE_RADIANS
      SII_LOCAL_do_assoc("\\hbar", 5, 1.054571800e-34 , (2,-1,1,0,0,0,0,-1) );
      SII_LOCAL_do_assoc("\\hslash", 7, 1.054571800e-34 , (2,-1,1,0,0,0,0,-1) );
#else
      SII_LOCAL_do_assoc("\\hbar", 5, 1.054571800e-34 , (2,-1,1) );
      SII_LOCAL_do_assoc("\\hslash", 7, 1.054571800e-34 , (2,-1,1) );
#endif

      SII_LOCAL_do_assoc("m_e", 3, 9.10938356e-31, (0,0,1) );
      SII_LOCAL_do_assoc("me", 2, 9.10938356e-31, (0,0,1) );
      SII_LOCAL_do_assoc("e", 1, 1.6021766208e-19 , (0,1,0,0,1) );

      SII_LOCAL_do_assoc("a_0", 3, 5.2917721067e-11, (1) );
      SII_LOCAL_do_assoc("a0", 2, 5.2917721067e-11, (1) );

      SII_LOCAL_do_assoc("E_h", 3, 4.359744650e-18, (2,-2,1) );
      SII_LOCAL_do_assoc("Eh", 2, 4.359744650e-18, (2,-2,1) );

      SII_LOCAL_do_assoc("NM", 2, 1852, (1) );
      SII_LOCAL_do_assoc("M", 1, 1852, (1) );
      //SII_LOCAL_do_assoc("nmi", 3, 1852, (1) );
      //This symbol might conflict with nano-miles...


      SII_LOCAL_do_assoc("kn", 2, 1852.0/3600.0, (1,-1) );


      SII_LOCAL_do_assoc("\\AA", 3, 1e-10, (1) );
      SII_LOCAL_do_assoc("\\r{A}", 5, 1e-10, (1) );

      SII_LOCAL_do_assoc("b", 1, 1e-28, (2) );

      SII_LOCAL_do_assoc("bar", 3, 100000, (-1,-2,1) );

      SII_LOCAL_do_assoc("mmHg", 4, 133.322387415, (-1,-2,1) );

#if USE_RADIANS
      SII_LOCAL_do_assoc("Ci", 2, 3.7e10, (0,-1,0,0,0,0,0,0,0,true) );
#else
      SII_LOCAL_do_assoc("Ci", 2, 3.7e10, (0,-1,0,0,0,0,0,true) );
#endif
      SII_LOCAL_do_assoc("R", 1, 2.58e-4, (0,1,-1,0,1) );

#if USE_RADIANS
      SII_LOCAL_do_assoc("rd", 2, 1e-2, (2,-2,0,0,0,0,0,0,0,false,true) );
#else
      SII_LOCAL_do_assoc("rd", 2, 1e-2, (2,-2,0,0,0,0,0,false,true) );
#endif
      //So as not to conflict with radians

      SII_LOCAL_do_assoc("rem", 2, 1e-2, (2,-2) );

      SII_LOCAL_do_assoc("erg", 3, 1e-7, (2,-2,1) );

      SII_LOCAL_do_assoc("dyn", 3, 1e-5, (1,-2,1) );

      SII_LOCAL_do_assoc("P", 1, 0.1, (-1,-1,1) );

      SII_LOCAL_do_assoc("St", 2, 1e-4, (2,-1) );

      SII_LOCAL_do_assoc("G", 1, 1e-4, (0,-2,1,0,-1) );
      //SII_LOCAL_do_assoc("Gs", 2, 1e-4, (0,-2,1,0,-1) );
      //This can conflict with Giga-seconds.

      SII_LOCAL_do_assoc("Oe", 2, 1000.0/(4*pi), (-1,0,0,0,1) );

      SII_LOCAL_do_assoc("Mx", 2, 1e-8, (2,-2,1,0,-1) );

      SII_LOCAL_do_assoc("sb", 2, 1,   (0,0,0,0,0,1) );

#if USE_RADIANS
      SII_LOCAL_do_assoc("ph", 2, 10000, (-2,0,0,0,0,1,0,0,1) );
#else
      SII_LOCAL_do_assoc("ph", 2, 10000, (-2,0,0,0,0,1) );
#endif

      SII_LOCAL_do_assoc("Gal", 3, 1e-2, (1,-2) );

      SII_LOCAL_do_assoc("cal", 3, 4.184, (2,-2,1) );

      SII_LOCAL_do_assoc("\\epsilon_0", 10, 625000.0/2246887946420441.0/pi, (-3,4,-1,0,2) );
      SII_LOCAL_do_assoc("epsilon_0", 9, 625000.0/2246887946420441.0/pi, (-3,4,-1,0,2) );
      SII_LOCAL_do_assoc("varepsilon_0", 12, 625000.0/2246887946420441.0/pi, (-3,4,-1,0,2) );
      SII_LOCAL_do_assoc("\\varepsilon_0", 13, 625000.0/2246887946420441.0/pi, (-3,4,-1,0,2) );

      SII_LOCAL_do_assoc("\\mu_0", 5, (4*pi*1e-7), (1,-2,1,0,-2) );
      SII_LOCAL_do_assoc("mu_0", 4, (4*pi*1e-7), (1,-2,1,0,-2) );

    }

    void add_SI_units() const
    {
      In key;
      SI_value<Out, In, Hold> val;

      assoc[key] = val;

      //Main SI units...
      SII_LOCAL_do_assoc("m", 1, 1,    (1) );
      SII_LOCAL_do_assoc("s", 1, 1,    (0,1) );
      SII_LOCAL_do_assoc("g", 1, 1e-3, (0,0,1) ); //The usual deal...
      SII_LOCAL_do_assoc("K", 1, 1,    (0,0,0,1) );
      SII_LOCAL_do_assoc("A", 1, 1,    (0,0,0,0,1) );
      SII_LOCAL_do_assoc("cd", 2, 1,   (0,0,0,0,0,1) );
      SII_LOCAL_do_assoc("mol", 3, 1,  (0,0,0,0,0,0,1) );
#if USE_RADIANS
      SII_LOCAL_do_assoc("rad", 3, 1,  (0,0,0,0,0,0,0,1) );
      SII_LOCAL_do_assoc("sr", 2, 1,   (0,0,0,0,0,0,0,0,1) );
#endif

      //Derived SI units...
      SII_LOCAL_do_assoc("Hz", 2, 1, (0,-1) );
      SII_LOCAL_do_assoc("N", 1, 1, (1,-2,1) );
      SII_LOCAL_do_assoc("Pa", 2, 1, (-1,-2,1) );
      SII_LOCAL_do_assoc("J", 1, 1, (2,-2,1) );
      SII_LOCAL_do_assoc("W", 1, 1, (2,-3,1) );
      SII_LOCAL_do_assoc("C", 1, 1, (0,1,0,0,1) );
      SII_LOCAL_do_assoc("V", 1, 1, (2,-3,1,0,-1) );
      SII_LOCAL_do_assoc("F", 1, 1, (-2,4,-1,0,2) );
      SII_LOCAL_do_assoc("\\Omega", 6, 1, (2,-3,1,0,-2) );
      SII_LOCAL_do_assoc("Ohm", 3, 1, (2,-3,1,0,-2) );
      SII_LOCAL_do_assoc("O", 1, 1, (2,-3,1,0,-2) );//Alternative for Ohm
      SII_LOCAL_do_assoc("S", 1, 1, (-2,3,-1,0,2) );
      SII_LOCAL_do_assoc("Wb", 2, 1, (2,-2,1,0,-1) );
      SII_LOCAL_do_assoc("T", 1, 1, (0,-2,1,0,-1) );
      SII_LOCAL_do_assoc("H", 1, 1, (2,-2,1,0,-2) );
      SII_LOCAL_do_assoc("oC", 2, 1, (0,0,0,1) );
      //The degree symbol can be semi-problematic...
      SII_LOCAL_do_assoc("\\degC", 5, 1, (0,0,0,1) );
      SII_LOCAL_do_assoc("\\Celsius", 8, 1, (0,0,0,1) );
      //Let's skip converting degrees, for now, given that they're not exactly linear...
#if USE_RADIANS
      SII_LOCAL_do_assoc("lm", 2, 1, (0,0,0,0,0,1,0,0,1) );
      SII_LOCAL_do_assoc("lx", 2, 1, (-2,0,0,0,0,1,0,0,1) );
      SII_LOCAL_do_assoc("Bq", 2, 1, (0,-1,0,0,0,0,0,0,0,true) );
      SII_LOCAL_do_assoc("Gy", 2, 1, (2,-2,0,0,0,0,0,0,0,false,true) );
#else
      SII_LOCAL_do_assoc("lm", 2, 1, (0,0,0,0,0,1) );
      SII_LOCAL_do_assoc("lx", 2, 1, (-2,0,0,0,0,1) );
      SII_LOCAL_do_assoc("Bq", 2, 1, (0,-1,0,0,0,0,0,true) );
      SII_LOCAL_do_assoc("Gy", 2, 1, (2,-2,0,0,0,0,0,false,true) );
#endif
      SII_LOCAL_do_assoc("Sv", 2, 1, (2,-2) );
      SII_LOCAL_do_assoc("kat", 3, 1, (0,-1,0,0,0,1) );

    }

#undef SII_LOCAL_do_assoc

#ifdef SII_LOCAL_do_prefix
#error For SII to work properly, the macro 'SII_LOCAL_do_prefix' musn't be defined. Please change your macro's name and/or the order in which you include SII's code (the macro doesn't remain defined after the code, so the name can still be used).
#endif

#define SII_LOCAL_do_prefix(STR, SIZE, VALUE)   \
    {                                           \
      key.clear();                              \
      update_stringlike(key, STR, SIZE);        \
      prefix[key] = (Out) VALUE;                \
    }                                           \


    void add_SI_prefixes() const
    {
      In key;

      prefix[key] = 1;

      SII_LOCAL_do_prefix("Y", 1, 1e24);
      SII_LOCAL_do_prefix("Z", 1, 1e21);
      SII_LOCAL_do_prefix("E", 1, 1e18);
      SII_LOCAL_do_prefix("P", 1, 1e15);
      SII_LOCAL_do_prefix("T", 1, 1e12);
      SII_LOCAL_do_prefix("G", 1, 1e9);
      SII_LOCAL_do_prefix("M", 1, 1e6);
      SII_LOCAL_do_prefix("k", 1, 1e3);
      SII_LOCAL_do_prefix("h", 1, 1e2);
      SII_LOCAL_do_prefix("da", 2, 1e1);
      SII_LOCAL_do_prefix("d", 1, 1e-1);
      SII_LOCAL_do_prefix("c", 1, 1e-2);
      SII_LOCAL_do_prefix("m", 1, 1e-3);
      SII_LOCAL_do_prefix("\\mu", 3, 1e-6);
      SII_LOCAL_do_prefix("mu", 2, 1e-6);
      SII_LOCAL_do_prefix("n", 1, 1e-9);
      SII_LOCAL_do_prefix("p", 1, 1e-12);
      SII_LOCAL_do_prefix("f", 1, 1e-15);
      SII_LOCAL_do_prefix("a", 1, 1e-18);
      SII_LOCAL_do_prefix("z", 1, 1e-21);
      SII_LOCAL_do_prefix("y", 1, 1e-24);
      largest_prefix = 3;
    }

#undef SII_LOCAL_do_prefix
  private:

    SI_value<Out, In, Hold> string_to_value(const In &s) const
    {
      size_t i;
      Out temp = 1;
      if(std::isdigit(s[0]))
      {
        std::basic_stringstream <typename In::value_type> stream(s);
        stream >> temp;
      }
      for(i = 0; i <= largest_prefix && i < s.size(); ++i)
        {
          if(assoc.count(s.substr(i)) && prefix.count(s.substr(0,i)))
            {
              return temp * assoc[s.substr(i)] * (i ? prefix[s.substr(0,i)] : 1);
            }
        }
      return SI_value<Out, In, Hold>(temp);
    }

    SI_value<Out, In, Hold> interpret_string(const In &s) const
    {

      std::basic_stringstream<typename In::value_type> stream(s);

      SI_value<Out, In, Hold> tempv, ret;
      In temps;
      int tempint;
      //If true, the return value should be divided by the next interpreted value.
      std::vector<In> delims(3);
      update_stringlike(delims[0], "\\times", 5);
      update_stringlike(delims[1], " ", 1);
      update_stringlike(delims[2], "\\cdot", 5);

      stream >> std::ws;
      if(isdigit(stream.peek()))
        {
          stream >> ret.value;
          if(stream.fail())
            {
              stream.clear();
              stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
            }
        }
      while(stream.good() && ret.value)
        {
          stream >> std::ws;
          if(stream.peek() == '(')
            {
              stream.get();
              get_with_par(stream, temps, (typename In::value_type) ')', false);
              tempv = interpret_string(temps);
            }
          else if(stream.peek() == '[')
            {
              stream.get();
              get_with_par(stream, temps, (typename In::value_type) ']', false);
              tempv = interpret_string(temps);
            }
          else if(stream.peek() == '{')
            {
              stream.get();
              get_with_par(stream, temps, (typename In::value_type) '}', false);
              tempv = interpret_string(temps);
            }
          else if(stream.peek() == '^')
            {
              tempint = 0;
              stream.get();
              if(stream.peek() == '(')
                {
                  stream.get();
                  stream >> tempint;
                  stream.clear();
                  stream.ignore(std::numeric_limits<std::streamsize>::max(), ')');
                }
              else if(stream.peek() == '[')
                {
                  stream.get();
                  stream >> tempint;
                  stream.clear();
                  stream.ignore(std::numeric_limits<std::streamsize>::max(), ']');
                }
              else if(stream.peek() == '{')
                {
                  stream.get();
                  stream >> tempint;
                  stream.clear();
                  stream.ignore(std::numeric_limits<std::streamsize>::max(), '}');
                }
              else
                {
                  stream >> tempint;
                  stream.clear();
                  stream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
                }
              if (tempint > 0)
                {
                  ret *= fastpow(tempv, abs(tempint));
                }
              else if (tempint < 0)
                {
                  ret /= fastpow(tempv, abs(tempint));
                }
              tempv.reset();
            }
          else if(stream.peek() == '/')
            {
              stream.get();
              stream >> std::ws;
              get_with_par(stream, temps, delims, false);
              tempv /= interpret_string(temps);
            }
          else
            {
              temps.clear();
              while( stream.peek() != '/' && stream.peek() != ' ' && stream.peek() != '(' &&
                     stream.peek() != '[' && stream.peek() != '{' && stream.peek() != '^' &&
                     stream.good()                                                           )
                {
                  temps += stream.get();
                }
              ret *= tempv;
              tempv = string_to_value(temps);
            }
        }
      return ret*tempv;
    }

  public:

    void add_prefix(const In& key, const Out& value)
    {
      prefix[key] = value;
      largest_prefix = std::max(largest_prefix, key.size());
    }

    void add_unit(const In& key, const SI_value<Out, In, Hold> &unit)
    {
      assoc[key] = unit;
    }

    SI_interpreter(const bool more_units = true)
    {
      add_SI_units();
      if(more_units)
      {
        add_more_units();
      }
      add_SI_prefixes();
    }

    SI_value<Out, In, Hold> interpret(const In &s) const
    {
      std::basic_stringstream<typename In::value_type> stream(s);
      Out temp;
      stream >> temp;
      if(stream.fail())
      {
        return interpret_string(s);
      }
      else if(stream.tellg() < s.size())
      {
        return temp*interpret_string(s.substr(stream.tellg()));
      }
      else
      {
        return SI_value<Out,In,Hold>(temp);
      }
    }

    SI_interpreter& operator= (const SI_interpreter &x)
    {
      if(&x != this)
        {
          assoc = x.assoc;
          prefix = x.prefix;
        }
      return (*this);
    }

  };

}

#endif
