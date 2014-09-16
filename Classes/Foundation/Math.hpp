/****************************************************************************
  Copyright (c) 2014-2015 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef Foundation_Math_h
#define Foundation_Math_h
#pragma  once

#include <vector>
#include <limits>
#include <cmath>

namespace Foundation{

    /// Provides constants and static methods for trigonometric, 
    /// logarithmic and other common mathematical functions.
    namespace Math
    {
        /// A few useful constants
        const int     MaxInt    = (std::numeric_limits<int>::max)();
        const double  MaxDouble = (std::numeric_limits<double>::max)();
        const double  MinDouble = (std::numeric_limits<double>::min)();
        const float   MaxFloat  = (std::numeric_limits<float>::max)();
        const float   MinFloat  = (std::numeric_limits<float>::min)();

        /// Degrees-to-radians conversion constant.
        const float Deg2Rad = 0.0174533f;

        /// Degrees-to-grad conversion constant.
        const float Deg2Grad = 1.1111111f;

        /// A tiny floating point value.
        const float Epsilon = 1.4013e-045f;

        /// Exponential e.
        const float ExponentialE = 2.71828f;

        /// The golden ratio. Oooooh!
        const float GoldenRatio = 1.61803f;

        /// Grad-to-degrees conversion constant.
        const float Grad2Deg = 0.9f;

        /// Grad-to-radians conversion constant.
        const float Grad2Rad = 0.015708f;

        /// A representation of positive infinity.
        const float Infinity = MaxFloat;// 1.0f / 0.0f;

        /// A representation of negative infinity.
        const float NegativeInfinity = MinFloat;// -1.0f / 0.0f;

        /// The infamous 3.14159265358979... value.
        const float PI = 3.14159f;

        /// Radians-to-degrees conversion constant.
        const float Rad2Deg = 57.2958f;

        /// Radians-to-grad conversion constant.
        const float Rad2Grad = 63.6619772f;

        const double   TwoPi     = PI * 2;
        const double   HalfPi    = PI / 2;
        const double   QuarterPi = PI / 4;

        /// Returns the absolute value of val.
        template< typename T >
        inline T abs(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        {return std::abs(val);}

        /// Returns the arc-cosine of val - the angle in radians whose cosine is val.
        template< typename T >
        inline T acos(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        {return std::acos(val);}

        /** 
         * Compares two values if they are similar.
         * Due to floating point imprecision it is not 
         * recommended to compare floats using the equal
         * operator. eg. 1.0 == 10.0 / 10.0 might not return true. 
         */
        template< typename T1, typename T2 >
        bool approximately(const T1 a,const T2 b,
            typename std::enable_if< std::is_arithmetic<T1>::value >::type* = 0,
            typename std::enable_if< std::is_arithmetic<T2>::value >::type* = 0) 
        { return roughlyEqual(a, b, 0.1f);  }

        /// Returns the arc-sine of val - the angle in radians whose sine is val.
        template< typename T >
        T asin(const T val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::asin(val); }

        /// Returns the arc-tangent of val - the angle in radians whose tangent is val.
        template< typename T >
        T atan(const T val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::atan(val); }

        /// Returns the angle in radians whose Tan is y/x.
        template< typename T >
        T atan2(const T y, const T x,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        { return std::atan2(y, x); }

        /// Returns the smallest integer greater to or equal to val.
        template< typename T >
        T ceil(const T val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::ceil(val); }

        /// Returns the smallest integer greater to or equal to val.
        template< typename T >
        int ceilToInt(const T val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        {return static_cast<int>(std::ceil(val)); }

        /// Clamps val value between a minimum and maximum value.
        template< typename T, typename U, typename V >
        inline T clamp(const T& val, const U& minVal, const V& maxVal,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0,
            typename std::enable_if< std::is_arithmetic<U>::value >::type* = 0,
            typename std::enable_if< std::is_arithmetic<V>::value >::type* = 0)
        {
            if (maxVal <= minVal)
                return minVal;
            return val < minVal ? minVal : val > maxVal ? maxVal : val;
        }

        /// Clamps value between 0 and 1 and returns value.
        template< typename T>
        inline T clamp01(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        { return val < 0 ? 0 : val > 1 ? 1 : value; }

        /// Returns the cosine of angle f in radians.
        template< typename T >
        T cos(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::cos(val); }

        /// Returns e raised to the specified power.
        template< typename T >
        T exp(const T& power,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::exp(power); }

        /// Returns the largest integer smaller to or equal to val.
        template< typename T >
        T floor(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        { return std::floor(val); }

        /// Returns the largest integer smaller to or equal to val.
        template< typename T >
        int floorToInt(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return static_cast<int>(std::floor(val)); }

        /// Interpolates between from and to by t. t is clamped between 0 and 1.
        template< typename T, typename U, typename V>
        float Lerp(const T& from, const U& to, const V& t,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0,
            typename std::enable_if< std::is_arithmetic<U>::value >::type* = 0,
            typename std::enable_if< std::is_arithmetic<V>::value >::type* = 0) 
        { 
            return t >= 1 ? to : t < 0 ? from : from + (to - from) * t; 
        }

        /// Returns the natural (base e) logarithm of val specified value.
        template< typename T >
        T Log(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::log(val); }

        /// Returns the base 10 logarithm of val specified value.
        template< typename T >
        T Log10(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        { return std::log10(val); }

        /// returns the maximum of two values
        template< typename T >
        inline T max(const T& v1, const T& v2,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        { return v1 > v2 ? v1 : v2; }

        /// returns the minimum of two values
        template <typename T>
        inline T min(const T& a, const T& b,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        { return a < b ? a : b; }

        /// Returns f raised to power p.
        template< typename T , typename U >
        T Pow(const T& f, const U& p,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0,
            typename std::enable_if< std::is_arithmetic<U>::value >::type* = 0)
        { return std::pow(f, p); }

        /// Get the next power of two after val value.
        static int nextPowerOfTwo(int val) 
        {
            if (val < 0)
                return 0;
            val |= val >> 1;
            val |= val >> 2;
            val |= val >> 4;
            val |= val >> 8;
            val |= val >> 16;
            return val + 1;
        }

        /// Returns the closest power of two to val value.
        static int closestPowerOfTwo(int val) 
        {
            int b = nextPowerOfTwo(val);
            int c = b / 2;
            return val - c < b - val ? c : b;
        }


        /// Returns if the value is powered by two.
        static bool isPowerOfTwo(const int& val) 
        {
            return (val > 0) && ((val & (val - 1)) == 0);
        }

        /**
         * Compares two floating point values if they are similar.
         * @param[in] v1 First value.
         * @param[in] v2 Second value.
         * @param[in] threshold The threshold of similarity
         * @return True if the values are similar, otherwise false.
         */
        template< typename T, typename U, typename V>
        bool roughlyEqual(const T v1, const U v2, const V threshold = 0.01f) 
        { return abs(v1 - v2) <= threshold;  }

        /// Returns f rounded to the nearest integer.
        template <typename T>
        T Round(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::round(val); }

      

        /** 
        /// <summary>
        /// Rounds a floating-point value to a specified number of fractional digits. 
        /// </summary>
        /// <param name="f">The value</param>
        /// <param name="decimals">The number of fractional digits to round to</param>
        float Round(float f, int decimals) { return std::round(f, decimals); }

        /// <summary>
        /// Rounds a floating-point value to a specified number of fractional digits. A parameter specifies how to round a value if it is midway between two other numbers.
        /// </summary>
        /// <param name="f">The value</param>
        /// <param name="decimals">The number of fractional digits to round to</param>
        /// <param name="mode">The rounding mode to use</param>
        public static float Round(float f, int decimals, MidpointRounding mode) { return (float)Math.Round(f, decimals, mode); }
        /// <summary>
        /// Returns f rounded to the nearest integer.
        /// </summary>
        /// <param name="f">The value to round</param>
        public static int RoundToInt(float f) { return (int)Round(f); }
        /// <summary>
        ///  Rounds a floating-point value to a specified number of fractional digits. Except not. This method makes no sense.
        /// </summary>
        /// <param name="f">The value</param>
        /// <param name="decimals">The number of fractional digits to round to</param>
        public static int RoundToInt(float f, int decimals) { return (int)Round(f, decimals); }
        /// <summary>
        /// Rounds a floating-point value to a specified number of fractional digits. A parameter specifies how to round a value if it is midway between two other numbers. Except not. This method makes no sense.
        /// </summary>
        /// <param name="f">The value</param>
        /// <param name="decimals">The number of fractional digits to round to</param>
        /// <param name="mode">The rounding mode to use</param>
        public static int RoundToInt(float f, int decimals, MidpointRounding mode) { return (int)Round(f, decimals, mode); }

        **/

        /// Returns the sign of val.
        template <typename T>
        T sign(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        { return std::sign(val); }

        /// Returns the sine of angle val in radians.
        template <typename T>
        T sin(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::sin(val); }

        /// Returns square root of val.
        template <typename T>
        T sqrt(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::sqrt(val); }

        /// Returns the tangent of angle val in radians.
        template <typename T>
        T tan(const T& val,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0) 
        { return std::tan(val); }

        inline bool equal(const float& v1, const float& v2)
        {
            return (std::fabs(v1-v2) < 1E-12);
        }

        inline bool equal(const double& v1, const double& v2)
        {
            return (std::fabs(v1-v2) < 1E-12);
        }

        template< typename T >
        inline bool fuzzyEqual(const T& v1, const T& v2, const T& var)
        {
            return(v1 - var <= v2 && v2 <= v1 + var);
        }

        /// Return true if the parameter is equal to zero
        inline bool isZero(const double& val)
        {
            return ((-MinDouble < val) && (val < MinDouble));
        }

        template<typename T>
        inline double average(const std::vector<T>& vec,
            typename std::enable_if< std::is_arithmetic<T>::value >::type* = 0)
        {
            double dAverage = 0.0;
            for (auto& i : vec )
            {
                dAverage += static_cast<double>(i);
            }
            return dAverage / static_cast<double>(vec.size());
        }

        inline double standardDeviation(const std::vector<double>& vec)
        {
            double sd = 0.0;
            double dAverage = average<double>(vec);
            for (unsigned int i=0; i < vec.size(); ++i)
            {
                sd +=(vec[i]- dAverage)*(vec[i] - dAverage);
            }

            sd = sd / vec.size();
            return sqrt(sd);
        }

        inline double sigmoid(const double& input, const double& response = 1.0)
        {
            return ( 1.0 / ( 1.0 + exp(-input / response)));
        }

        // return true is the third parameter is in the range described by the first two
        template< typename T, typename U, typename V>
        inline bool inRange(const T& start, const U& end, const V& val)
        {
            if (start < end)
            {
                return ((val > start) && (val < end));
            }
            else
            {
                return ((val < start) && (val > end));
            }
        }

    } // namespace Math

} // namespace Foundation

#endif // Foundation_Math_h

















