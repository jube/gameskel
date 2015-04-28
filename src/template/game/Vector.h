/*
 * Copyright (c) 2014-2015, Julien Bernard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef GAME_VECTOR_H
#define GAME_VECTOR_H

namespace game {

  /**
   * @ingroup base
   */
  struct Vector2f {
    float x;
    float y;
  };

  // equality
  /**
   * @ingroup base
   */
  inline
  bool operator==(const Vector2f& lhs, const Vector2f& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
  }

  // unary minus
  /**
   * @ingroup base
   */
  inline
  Vector2f operator-(const Vector2f& v) {
    return { - v.x, - v.y };
  }

  // addition
  /**
   * @ingroup base
   */
  inline
  Vector2f operator+(const Vector2f& lhs, const Vector2f& rhs) {
    return { lhs.x + rhs.x, lhs.y + rhs.y };
  }

  // addition and assignment
  /**
   * @ingroup base
   */
  inline
  Vector2f& operator+=(Vector2f& lhs, const Vector2f& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
  }

  // substraction
  /**
   * @ingroup base
   */
  inline
  Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
  }

  // substraction and assignment
  /**
   * @ingroup base
   */
  inline
  Vector2f& operator-=(Vector2f& lhs, const Vector2f& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
  }

  // scalar multiplication (on left)
  /**
   * @ingroup base
   */
  inline
  Vector2f operator*(float lhs, const Vector2f& rhs) {
    return { lhs * rhs.x, lhs * rhs.y };
  }

  // scalar multiplication (on right)
  inline
  /**
   * @ingroup base
   */
  Vector2f operator*(const Vector2f& lhs, float rhs) {
    return { lhs.x * rhs, lhs.y * rhs };
  }

  // scalar multiplication and assignment
  /**
   * @ingroup base
   */
  inline
  Vector2f& operator*=(Vector2f& lhs, float rhs) {
    lhs.x *= rhs;
    lhs.x *= rhs;
    return lhs;
  }

  // scalar division
  /**
   * @ingroup base
   */
  inline
  Vector2f operator/(const Vector2f& lhs, float rhs) {
    return { lhs.x / rhs, lhs.y / rhs };
  }

  // scalar division and assignment
  /**
   * @ingroup base
   */
  inline
  Vector2f& operator/=(Vector2f& lhs, float rhs) {
    lhs.x /= rhs;
    lhs.x /= rhs;
    return lhs;
  }

}


#endif // GAME_VECTOR_H
