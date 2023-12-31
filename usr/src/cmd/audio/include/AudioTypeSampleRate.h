/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Copyright (c) 1992-2001 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef _MULTIMEDIA_AUDIOTYPESAMPLERATE_H
#define	_MULTIMEDIA_AUDIOTYPESAMPLERATE_H

#include <AudioTypeConvert.h>
#include <Resample.h>

#ifdef __cplusplus
extern "C" {
#endif

// This is the class doing Sample Rate conversion

class AudioTypeSampleRate : public AudioTypeConvert {
private:
	ResampleFilter	resampler;
	int 		input_rate;
	int		output_rate;

protected:

public:
	AudioTypeSampleRate(int inrate, int outrate); // Constructor
	~AudioTypeSampleRate();				// Destructor

	// Class AudioTypeConvert methods specialized here
	// TRUE if conversion ok
	virtual Boolean CanConvert(
	    AudioHdr h) const;			// type to check against

	// Convert buffer to the specified type
	// Either the input or output type must be handled by this class
	// Convert to new type
	virtual AudioError Convert(
	    AudioBuffer*& inbuf,		// data buffer to process
	    AudioHdr outhdr);			// target header

	virtual AudioError Flush(AudioBuffer*& buf);	// flush remains
};

#ifdef __cplusplus
}
#endif

#endif /* !_MULTIMEDIA_AUDIOTYPESAMPLERATE_H */
