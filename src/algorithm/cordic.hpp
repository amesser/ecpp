/*
 *  Copyright 2014 Andreas Messer <andi@bastelmap.de>
 *
 *  This file is part of the Embedded C++ Platform Project.
 *
 *  Embedded C++ Platform Project (ECPP) is free software: you can
 *  redistribute it and/or modify it under the terms of the GNU General
 *  Public License as published by the Free Software Foundation,
 *  either version 3 of the License, or (at your option) any later
 *  version.
 *
 *  Embedded C++ Platform Project is distributed in the hope that it
 *  will be useful, but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ECPP.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  As a special exception, the copyright holders of ECPP give you
 *  permission to link ECPP with independent modules to produce an
 *  executable, regardless of the license terms of these independent
 *  modules, and to copy and distribute the resulting executable under
 *  terms of your choice, provided that you also meet, for each linked
 *  independent module, the terms and conditions of the license of that
 *  module.  An independent module is a module which is not derived from
 *  or based on ECPP.  If you modify ECPP, you may extend this exception
 *  to your version of ECPP, but you are not obligated to do so.  If you
 *  do not wish to do so, delete this exception statement from your
 *  version.
 *  */
#ifndef CORDIC_HPP_
#define CORDIC_HPP_

namespace Platform {
  namespace Algorithm {
    class CordicState
    {
    public:
      double m_x;
      double m_y;
      double m_z;
    };

    template<unsigned long INDEX>
    static constexpr double CordicArctan();

    template<> constexpr double CordicArctan<0>() {return 0.785398163397;}
    template<> constexpr double CordicArctan<1>() { return 0.463647609001;}
    template<> constexpr double CordicArctan<2>() { return 0.244978663127;}
    template<> constexpr double CordicArctan<3>() { return 0.124354994547;}
    template<> constexpr double CordicArctan<4>() { return 0.062418809996;}
    template<> constexpr double CordicArctan<5>() { return 0.0312398334303;}
    template<> constexpr double CordicArctan<6>() { return 0.0156237286205;}
    template<> constexpr double CordicArctan<7>() { return 0.0078123410601;}
    template<> constexpr double CordicArctan<8>() { return 0.00390623013197;}
    template<> constexpr double CordicArctan<9>() { return 0.00195312251648;}
    template<> constexpr double CordicArctan<10>() { return 0.000976562189559;}
    template<> constexpr double CordicArctan<11>() { return 0.000488281211195;}
    template<> constexpr double CordicArctan<12>() { return 0.000244140620149;}
    template<> constexpr double CordicArctan<13>() { return 0.000122070311894;}
    template<> constexpr double CordicArctan<14>() { return 6.10351561742e-05;}
    template<> constexpr double CordicArctan<15>() { return 3.05175781155e-05;}
    template<> constexpr double CordicArctan<16>() { return 1.52587890613e-05;}
    template<> constexpr double CordicArctan<17>() { return 7.6293945311e-06;}
    template<> constexpr double CordicArctan<18>() { return 3.81469726561e-06;}
    template<> constexpr double CordicArctan<19>() { return 1.90734863281e-06;}
    template<> constexpr double CordicArctan<20>() { return 9.53674316406e-07;}
    template<> constexpr double CordicArctan<21>() { return 4.76837158203e-07;}
    template<> constexpr double CordicArctan<22>() { return 2.38418579102e-07;}
    template<> constexpr double CordicArctan<23>() { return 1.19209289551e-07;}
    template<> constexpr double CordicArctan<24>() { return 5.96046447754e-08;}
    template<> constexpr double CordicArctan<25>() { return 2.98023223877e-08;}
    template<> constexpr double CordicArctan<26>() { return 1.49011611938e-08;}
    template<> constexpr double CordicArctan<27>() { return 7.45058059692e-09;}
    template<> constexpr double CordicArctan<28>() { return 3.72529029846e-09;}
    template<> constexpr double CordicArctan<29>() { return 1.86264514923e-09;}
    template<> constexpr double CordicArctan<30>() { return 9.31322574615e-10;}
    template<> constexpr double CordicArctan<31>() { return 4.65661287308e-10;}

    template<unsigned long INDEX>
    static constexpr double CordicNormalization();

    template<unsigned long INDEX>
    constexpr double CordicNormalization() {return 1.0;}

    template<> constexpr double CordicNormalization<0>  () { return 0.707106781187;}
    template<> constexpr double CordicNormalization<1>  () { return 0.894427191;}
    template<> constexpr double CordicNormalization<2>  () { return 0.970142500145;}
    template<> constexpr double CordicNormalization<3>  () { return 0.992277876714;}
    template<> constexpr double CordicNormalization<4>  () { return 0.998052578483;}
    template<> constexpr double CordicNormalization<5>  () { return 0.999512076087;}
    template<> constexpr double CordicNormalization<6>  () { return 0.999877952035;}
    template<> constexpr double CordicNormalization<7>  () { return 0.999969483819;}
    template<> constexpr double CordicNormalization<8>  () { return 0.999992370693;}
    template<> constexpr double CordicNormalization<9>  () { return 0.999998092657;}
    template<> constexpr double CordicNormalization<10> () { return 0.999999523163;}
    template<> constexpr double CordicNormalization<11> () { return 0.999999880791;}
    template<> constexpr double CordicNormalization<12> () { return 0.999999970198;}
    template<> constexpr double CordicNormalization<13> () { return 0.999999992549;}
    template<> constexpr double CordicNormalization<14> () { return 0.999999998137;}
    template<> constexpr double CordicNormalization<15> () { return 0.999999999534;}
    template<> constexpr double CordicNormalization<16> () { return 0.999999999884;}
    template<> constexpr double CordicNormalization<17> () { return 0.999999999971;}
    template<> constexpr double CordicNormalization<18> () { return 0.999999999993;}
    template<> constexpr double CordicNormalization<19> () { return 0.999999999998;}

    template<unsigned long ITERATIONS, unsigned long INDEX>
    class CordicRotator;

    template<unsigned long ITERATIONS, unsigned long INDEX>
    class CordicRotator
    {
    private:
      static constexpr double frac2()                  { return  1.0 / ((unsigned long)0x1 << INDEX);}
      static constexpr double sign(const double value) { return value >= 0 ? 1.0 : -1.0; }
      static constexpr double arctan();
    public:
      static constexpr CordicState rotate(CordicState s)
      {
        return CordicRotator<ITERATIONS, INDEX + 1>::rotate({
          (s.m_x - s.m_y * sign(s.m_z) * frac2()) * CordicNormalization<INDEX>(),
          (s.m_x * sign(s.m_z) * frac2() + s.m_y) * CordicNormalization<INDEX>(),
          s.m_z - sign(s.m_z) * CordicArctan<INDEX>(),
        });
      }
    };

    template<unsigned long ITERATIONS>
    class CordicRotator<ITERATIONS, ITERATIONS>
    {
    public:
      static constexpr CordicState rotate(CordicState s)
      {
        return s;
      }
    };


    template<unsigned long ITERATIONS = 32>
    class Cordic
    {
    public:
      static constexpr double m_pi = 3.141592653589793;
    private:
      static constexpr CordicState CordicArgTranslator(double arg)
      {
        return (arg > 2 * m_pi) ? CordicArgTranslator(arg - 2 * m_pi) :
               ((arg < 0) ? CordicArgTranslator(arg + 2 * m_pi) :
               ((arg > m_pi) ?
                   CordicState{0,-1,arg - 3 * m_pi / 2} :
                   CordicState{0, 1,arg - 1 * m_pi / 2}));
      }
    public:

      static constexpr double cos(double arg)
      {
        return CordicRotator<ITERATIONS,0>::rotate(CordicArgTranslator(arg)).m_x;
      }

      static constexpr double sin(double arg)
      {
        return CordicRotator<ITERATIONS,0>::rotate(CordicArgTranslator(arg)).m_y;

        /*
        return (CordicArgTranslator(arg) > m_pi) ?
            CordicRotator<ITERATIONS,0>::rotate({0,-1,CordicArgTranslator(arg) - 3 * m_pi / 2}).m_y:
            CordicRotator<ITERATIONS,0>::rotate({0,1,CordicArgTranslator(arg) - 1 * m_pi / 2}).m_y; */
      }
    };

  };
};



#endif /* CORDIC_HPP_ */
