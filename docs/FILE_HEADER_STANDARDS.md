# RudeBase3D File Header Standards

This document defines the standardized file header format used throughout the RudeBase3D codebase.

## Standard File Header Template

```cpp
//==============================================================================
// RudeBase3D Engine - filename.ext
//==============================================================================
// Professional 3D Modeling & Animation Suite
// https://github.com/The-No-Hands-company/RudeBase3D
//
// Copyright (c) 2025 The-No-Hands-company
// Licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to 
// deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//==============================================================================
```

## Alternative Compact Style

For smaller files or when space is a concern:

```cpp
//------------------------------------------------------------------------------
// RudeBase3D Engine | filename.ext
// Professional 3D Modeling Suite | MIT License
// Copyright (c) 2025 The-No-Hands-company
// https://github.com/The-No-Hands-company/RudeBase3D
//------------------------------------------------------------------------------
```

## Artistic Style (Optional)

For a more distinctive look:

```cpp
/*******************************************************************************
 * ████████╗   RudeBase3D Engine - filename.ext
 * ██╔══██║   Professional 3D Modeling & Animation Suite
 * ██████╔╝   https://github.com/The-No-Hands-company/RudeBase3D
 * ██╔══██╗   
 * ██║  ██║   Copyright (c) 2025 The-No-Hands-company | MIT License
 * ╚═╝  ╚═╝   
 ******************************************************************************/
```

## Design Philosophy

### RudeBase3D Identity
- **Double-line separators** (`//==============`) create strong visual boundaries
- **Engine branding** prominently displayed at the top
- **Professional tagline** reinforces the project's purpose
- **Clean typography** with clear information hierarchy

### Distinctive Features
- Uses `//` style comments instead of `/* */` blocks
- Emphasizes "RudeBase3D Engine" as a cohesive product name
- Includes descriptive tagline for immediate context
- Compact but comprehensive license information

### Visual Impact
- Strong horizontal lines create professional appearance
- Consistent 80-column width for terminal/editor compatibility
- Clear separation between project info and legal text
- Easily scannable information layout

## Usage Guidelines

### When to Apply
- All source files (.cpp, .h, .hpp)
- All header files in include/
- All implementation files in src/
- Build scripts and configuration files
- Documentation files where appropriate

### When NOT to Apply
- Third-party library files in third_party/
- Auto-generated files
- Temporary or experimental files clearly marked as such

### Customization Rules
1. Replace `filename.ext` with the actual filename
2. Keep the exact formatting and spacing
3. Maintain the 76-character width for the header lines
4. Use this header at the very top of each file, before any other content

### Benefits
- **Legal Protection**: Clear MIT license terms in every file
- **Professional Appearance**: Consistent with major open source projects
- **Easy Identification**: Quick file identification during development
- **Copyright Clarity**: Clear ownership and rights information
- **Search Friendly**: Easy to search for specific files using the header format

## Implementation Notes

- The header uses the MIT license (consistent with our LICENSE file)
- The GitHub URL provides easy access to the project
- The copyright notice includes "2025-present" to cover future years
- The formatting is optimized for 80-column displays
- The header provides immediate context about the project and file

## Example with Doxygen Documentation

```cpp
/**************************************************************************/
/*  mesh.hpp                                                              */
/**************************************************************************/
/*                         This file is part of:                         */
/*                             RUDEBASE3D                                 */
/*                     https://github.com/The-No-Hands-company/RudeBase3D */
/**************************************************************************/
/* Copyright (c) 2025-present The-No-Hands-company.                      */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

/**
 * @file mesh.hpp
 * @brief Core mesh representation and manipulation
 * 
 * This file contains the fundamental mesh classes used throughout
 * RudeBase3D for representing and manipulating 3D geometry.
 */

#pragma once
// ... rest of file content
```

This creates a clear separation between the legal header and the technical documentation, while maintaining professional standards throughout the codebase.
