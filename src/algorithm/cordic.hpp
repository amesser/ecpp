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

    template<> constexpr double CordicArctan<0>()  { return 0.785398163397;}
    template<> constexpr double CordicArctan<1>()  { return 0.463647609001;}
    template<> constexpr double CordicArctan<2>()  { return 0.244978663127;}
    template<> constexpr double CordicArctan<3>()  { return 0.124354994547;}
    template<> constexpr double CordicArctan<4>()  { return 0.062418809996;}
    template<> constexpr double CordicArctan<5>()  { return 0.0312398334303;}
    template<> constexpr double CordicArctan<6>()  { return 0.0156237286205;}
    template<> constexpr double CordicArctan<7>()  { return 0.0078123410601;}
    template<> constexpr double CordicArctan<8>()  { return 0.00390623013197;}
    template<> constexpr double CordicArctan<9>()  { return 0.00195312251648;}
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
    static constexpr double CordicArtanh();

    template<> constexpr double CordicArtanh<1> () { return 0.549306144334; }
    template<> constexpr double CordicArtanh<2> () { return 0.255412811883; }
    template<> constexpr double CordicArtanh<3> () { return 0.12565721414; }
    template<> constexpr double CordicArtanh<4> () { return 0.062581571477; }
    template<> constexpr double CordicArtanh<5> () { return 0.0312601784907; }
    template<> constexpr double CordicArtanh<6> () { return 0.0156262717521; }
    template<> constexpr double CordicArtanh<7> () { return 0.00781265895154; }
    template<> constexpr double CordicArtanh<8> () { return 0.0039062698684; }
    template<> constexpr double CordicArtanh<9> () { return 0.00195312748353; }
    template<> constexpr double CordicArtanh<10>() { return 0.000976562810441; }
    template<> constexpr double CordicArtanh<11>() { return 0.000488281288805; }
    template<> constexpr double CordicArtanh<12>() { return 0.000244140629851; }
    template<> constexpr double CordicArtanh<13>() { return 0.000122070313106; }
    template<> constexpr double CordicArtanh<14>() { return 6.10351563258e-05; }
    template<> constexpr double CordicArtanh<15>() { return 3.05175781345e-05; }
    template<> constexpr double CordicArtanh<16>() { return 1.52587890637e-05; }
    template<> constexpr double CordicArtanh<17>() { return 7.6293945314e-06; }
    template<> constexpr double CordicArtanh<18>() { return 3.81469726564e-06; }
    template<> constexpr double CordicArtanh<19>() { return 1.90734863281e-06; }
    template<> constexpr double CordicArtanh<20>() { return 9.53674316407e-07; }
    template<> constexpr double CordicArtanh<21>() { return 4.76837158203e-07; }
    template<> constexpr double CordicArtanh<22>() { return 2.38418579102e-07; }
    template<> constexpr double CordicArtanh<23>() { return 1.19209289551e-07; }
    template<> constexpr double CordicArtanh<24>() { return 5.96046447754e-08; }
    template<> constexpr double CordicArtanh<25>() { return 2.98023223877e-08; }
    template<> constexpr double CordicArtanh<26>() { return 1.49011611938e-08; }
    template<> constexpr double CordicArtanh<27>() { return 7.45058059692e-09; }
    template<> constexpr double CordicArtanh<28>() { return 3.72529029846e-09; }
    template<> constexpr double CordicArtanh<29>() { return 1.86264514923e-09; }
    template<> constexpr double CordicArtanh<30>() { return 9.31322574615e-10; }
    template<> constexpr double CordicArtanh<31>() { return 4.65661287308e-10; }
    template<> constexpr double CordicArtanh<32>() { return 2.32830643654e-10; }

    template<unsigned long INDEX>
    static constexpr double CordicTrigNormalization();

    template<unsigned long INDEX>
    constexpr double CordicTrigNormalization() {return 1.0;}

    template<> constexpr double CordicTrigNormalization<0>  () { return 0.707106781187;}
    template<> constexpr double CordicTrigNormalization<1>  () { return 0.894427191;}
    template<> constexpr double CordicTrigNormalization<2>  () { return 0.970142500145;}
    template<> constexpr double CordicTrigNormalization<3>  () { return 0.992277876714;}
    template<> constexpr double CordicTrigNormalization<4>  () { return 0.998052578483;}
    template<> constexpr double CordicTrigNormalization<5>  () { return 0.999512076087;}
    template<> constexpr double CordicTrigNormalization<6>  () { return 0.999877952035;}
    template<> constexpr double CordicTrigNormalization<7>  () { return 0.999969483819;}
    template<> constexpr double CordicTrigNormalization<8>  () { return 0.999992370693;}
    template<> constexpr double CordicTrigNormalization<9>  () { return 0.999998092657;}
    template<> constexpr double CordicTrigNormalization<10> () { return 0.999999523163;}
    template<> constexpr double CordicTrigNormalization<11> () { return 0.999999880791;}
    template<> constexpr double CordicTrigNormalization<12> () { return 0.999999970198;}
    template<> constexpr double CordicTrigNormalization<13> () { return 0.999999992549;}
    template<> constexpr double CordicTrigNormalization<14> () { return 0.999999998137;}
    template<> constexpr double CordicTrigNormalization<15> () { return 0.999999999534;}
    template<> constexpr double CordicTrigNormalization<16> () { return 0.999999999884;}
    template<> constexpr double CordicTrigNormalization<17> () { return 0.999999999971;}
    template<> constexpr double CordicTrigNormalization<18> () { return 0.999999999993;}
    template<> constexpr double CordicTrigNormalization<19> () { return 0.999999999998;}

    template<unsigned long INDEX>
    static constexpr double CordicHypNormalization();

    template<unsigned long INDEX>
    constexpr double CordicHypNormalization() {return 1.0;}

    template<> constexpr double CordicHypNormalization<1>  () { return 1.15470053838; }
    template<> constexpr double CordicHypNormalization<2>  () { return 1.03279555899; }
    template<> constexpr double CordicHypNormalization<3>  () { return 1.00790526136; }
    template<> constexpr double CordicHypNormalization<4>  () { return 1.00195886574; }
    template<> constexpr double CordicHypNormalization<5>  () { return 1.00048863917; }
    template<> constexpr double CordicHypNormalization<6>  () { return 1.00012209267; }
    template<> constexpr double CordicHypNormalization<7>  () { return 1.00003051898; }
    template<> constexpr double CordicHypNormalization<8>  () { return 1.00000762948; }
    template<> constexpr double CordicHypNormalization<9>  () { return 1.00000190735; }
    template<> constexpr double CordicHypNormalization<10> () { return 1.00000047684; }
    template<> constexpr double CordicHypNormalization<11> () { return 1.00000011921; }
    template<> constexpr double CordicHypNormalization<12> () { return 1.0000000298; }
    template<> constexpr double CordicHypNormalization<13> () { return 1.00000000745; }
    template<> constexpr double CordicHypNormalization<14> () { return 1.00000000186; }
    template<> constexpr double CordicHypNormalization<15> () { return 1.00000000047; }
    template<> constexpr double CordicHypNormalization<16> () { return 1.00000000012; }
    template<> constexpr double CordicHypNormalization<17> () { return 1.00000000003; }
    template<> constexpr double CordicHypNormalization<18> () { return 1.00000000001; }

    class CordicTrig
    {
    public:
      static constexpr double mode() {return 1.0;}

      template<unsigned long INDEX>
      static constexpr double normalization() {return CordicTrigNormalization<INDEX>();}

      template<unsigned long INDEX>
      static constexpr double angle() {return CordicArctan<INDEX>();}

      template<unsigned long INDEX>
      static constexpr double delta() { return  1.0 / ((unsigned long)0x1 << INDEX);}
    };

    class CordicLin
    {
    public:
      static constexpr double mode() {return 0.0;}

      template<unsigned long INDEX>
      static constexpr double normalization() {return 1.0;}

      template<unsigned long INDEX>
      static constexpr double delta() { return  1.0 / ((unsigned long)0x1 << INDEX);}

      template<unsigned long INDEX>
      static constexpr double angle() { return delta<INDEX>();}
    };

    template<unsigned long INDEX>
    unsigned long constexpr CordicHypIndex();

    template<unsigned long INDEX>
    unsigned long constexpr CordicHypIndex() {return CordicHypIndex<INDEX - 1>() + 1;}

    template<>
    unsigned long constexpr CordicHypIndex<0>() {return 1;}

    template<>
    unsigned long constexpr CordicHypIndex<4>() {return CordicHypIndex<3>();}

    template<>
    unsigned long constexpr CordicHypIndex<14>() {return CordicHypIndex<13>();}

    template<>
    unsigned long constexpr CordicHypIndex<42>() {return CordicHypIndex<41>();}

    class CordicHyp
    {
    public:
      static constexpr double mode() {return -1.0;}

      template<unsigned long INDEX>
      static constexpr double normalization() {return CordicHypNormalization< CordicHypIndex<INDEX>() >();}

      template<unsigned long INDEX>
      static constexpr double delta() { return  1.0 / ((unsigned long)0x1 << CordicHypIndex<INDEX>());}

      template<unsigned long INDEX>
      static constexpr double angle() {return CordicArtanh<CordicHypIndex<INDEX>()>();}
    };

    class CordicRotate
    {
    public:
      static constexpr double sigma(const CordicState &s) { return s.m_z >= 0 ? 1.0 : -1.0; }
    };

    class CordicVectoring
    {
    public:
      static constexpr double sigma(const CordicState &s) { return s.m_y > 0 ? -1.0 : +1.0; }
    };

    template<class MODE, class OPERATION, unsigned long ITERATIONS, unsigned long INDEX>
    class CordicRecurse;

    template<class MODE, class OPERATION, unsigned long ITERATIONS, unsigned long INDEX>
    class CordicRecurse
    {
    private:
      static constexpr double normalization()   {return MODE:: template normalization <INDEX>();}
      static constexpr double delta()           {return MODE:: template delta <INDEX>();}
      static constexpr double angle()           {return MODE:: template angle <INDEX> ();}
      static constexpr double mode()            {return MODE::mode();}
      static constexpr double sigma(const CordicState &s)         {return OPERATION::sigma(s);}
    public:
      static constexpr CordicState rotate(CordicState s)
      {
        return CordicRecurse<MODE, OPERATION, ITERATIONS, INDEX + 1>::rotate({
          (s.m_x - s.m_y * mode() * sigma(s) * delta()) * normalization(),
          (s.m_y + s.m_x * sigma(s) * delta())          * normalization(),
           s.m_z - sigma(s) * angle(),
        });
      }
    };

    constexpr long int CordicLog2(unsigned long int arg)
    {
      return (arg >= 2) ? CordicLog2(arg / 2) + 1 : 0;
    }

    constexpr long int CordicLog2(double arg)
    {
      return (arg >= 1) ? CordicLog2((unsigned long int)arg) : - CordicLog2((unsigned long int)(1.0/arg));
    }

    template<class MODE, class OPERATION, unsigned long ITERATIONS>
    class CordicRecurse<MODE, OPERATION, ITERATIONS, ITERATIONS>
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
      static constexpr double m_pi    = 3.141592653589793;
      static constexpr double m_sqrt2 = 1.4142135623730951;

    private:
      static constexpr CordicState CordicArgTranslator(double arg)
      {
        return (arg > 2 * m_pi) ? CordicArgTranslator(arg - 2 * m_pi) :
               ((arg <= 0) ? CordicArgTranslator(arg + 2 * m_pi) :
               ((arg > m_pi) ?
                   CordicState{0,-1,arg - 3 * m_pi / 2} :
                   CordicState{0,+1,arg - 1 * m_pi / 2}));
      }

      class SqrtArg
      {
      public:
        long   m_e;
        double m_arg;

        static constexpr double pow_2(long arg) {return (arg >= 0) ? (0x1 << arg) : (1.0 / (0x1 << (-arg)));}

        constexpr double rescale() const
        {
          return (m_arg > 0) ? pow_2( (m_e + 0x101) / 2 - 0x80) : 0;
        }

        constexpr double mantissa() const
        {
          return m_arg / pow_2(m_e) / ((m_e % 2) ? 2.0 : 1.0);
        }

        constexpr SqrtArg(double arg) : m_e((arg > 0) ? CordicLog2(arg) : 0), m_arg(arg) {}

        constexpr CordicState state() const
        {
          return {mantissa() + 0.25, mantissa() - 0.25, 0};
        }

      };


    public:

      static constexpr double cos(double arg)
      {
        return CordicRecurse<CordicTrig, CordicRotate, ITERATIONS,0>::rotate(CordicArgTranslator(arg)).m_x;
      }

      static constexpr double sin(double arg)
      {
        return CordicRecurse<CordicTrig, CordicRotate, ITERATIONS,0>::rotate(CordicArgTranslator(arg)).m_y;

        /*
        return (CordicArgTranslator(arg) > m_pi) ?
            CordicRotator<ITERATIONS,0>::rotate({0,-1,CordicArgTranslator(arg) - 3 * m_pi / 2}).m_y:
            CordicRotator<ITERATIONS,0>::rotate({0,1,CordicArgTranslator(arg) - 1 * m_pi / 2}).m_y; */
      }

      static constexpr double sqrt(double arg)
      {
        return SqrtArg(arg).rescale() * CordicRecurse<CordicHyp, CordicVectoring, ITERATIONS,0>::rotate(SqrtArg(arg).state()).m_x;
      }
    };

  };
};



#endif /* CORDIC_HPP_ */
