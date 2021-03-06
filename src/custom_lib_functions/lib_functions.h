#pragma once

#include "../objects/singular/osu_object.h"
#include "../objects/multiple/hit_object_v.h"
#include "../objects/multiple/timing_point_v.h"
#include "../exceptions/reamber_exception.h"
#include <vector>
// Here we declare all common functions that amber_base will include

//#ifdef AMBER_BASE_EX
//	#define AMBER_BASE __declspec(dllexport)                       
//#else
//	#define AMBER_BASE __declspec(dllimport)
//#endif

/*	REPRESENTING A VECTOR OF OSU_OBJECTs

	There are 2 ways to represent hit_object_v AND timing_point_v in a type
	
	1) std::shared_ptr<osu_object_v<T = hit_object/timing_point>> (Recommended)
		+ You get to use functions implemented in osu_object_v<T> class
		- Polymorphism may be a bit messy and confusing
		- Longer type name

	2) std::vector<T = hit_object/timing_point> 
		+ Shorter type name
		+ Simpler to use
		- No custom defined functions to use

	3) T = hit_object_v/timing_point_v
		+ Simple
		- Risky as T can be hit_object or timing_point too
		- Unclear on input

	For this library I've chosen the former so that I can utilize osu_object_v<T>'s own functions
	We are also able to shorten most library code and shift common and important implementations
		to the class itself
*/

namespace lib_functions 
{
	// Gets the difference in all offset difference in a vector form
	// Note that notes on the same offset will be regarded as 1 offset
	// This will return a vector that has a -1 size
	template <typename T>
	std::vector<double> get_offset_difference(osu_object_v<T> const* obj_v) {

		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}

		auto obj_v_copy = osu_object_v<T>::clone_obj_v(obj_v);

		obj_v_copy->sort_by_offset(true);
		double offset_buffer = obj_v->get_index(0).get_offset();
		std::vector<double> output = {};

		for (const T &obj : *obj_v_copy) {
			// If the offset is different, then we push the difference back to the output
			// We also set the offset_buffer as the new offset
			if (obj.get_offset() != offset_buffer) {
				output.push_back(obj.get_offset() - offset_buffer);
				offset_buffer = obj.get_offset();
			}
		}
		return output;
	}

	// Copies object to specified vector offsets
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies(T obj, std::vector<double> copy_to_v, bool sort = true) {
		osu_object_v<T> output = osu_object_v<T>();
		// For each offset to copy to
		for (double copy_to : copy_to_v) {
			obj.set_offset(copy_to);
			output.push_back(obj);
		}

		if (sort) {
			std::sort(output.begin(), output.end());
		}
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Copies objects to specified vector offsets
	// anchor_front defines if the start/end of the vector should be on the specified copy_to offset
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies(
		osu_object_v<T> const* obj_v, std::vector<double> copy_to_v,
		bool anchor_front = true, bool sort = true) {
		osu_object_v<T> output = osu_object_v<T>();

		auto obj_v_copy = osu_object_v<T>::clone_obj_v(obj_v);

		// For each offset to copy to 
		for (double copy_to : copy_to_v) {
			obj_v_copy->adjust_offset_to(copy_to, anchor_front);
			output.push_back(*obj_v_copy);
		}
		if (sort) {
			std::sort(output.begin(), output.end());
		}
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Divides the space in between each offset pair in offset_v then creates objects that segment it
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_subdivision(
		std::vector<double> offset_v, const T& obj_define,
		unsigned int subdivisions, bool include_with = true) {

		// If we don't want to include the initial offsets with it, we will do a blank vector
		// We create another vector of offset

		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}

		std::vector<double> offset_copy_to_v = include_with ? offset_v : std::vector<double>();

		// We extract the offsets on the subdivisions
		for (auto start = offset_v.begin(); start + 1 < offset_v.end(); start++) {
			
			// Eg. For 3 Subdivisions
			//     0   1   2   3   E
			//     O   |   |   |   O
			//     <--->
			//       ^ slice_distance
			double slice_distance = (*(start + 1) - *(start)) / subdivisions;

			// We start on 1 and end on <= due to multiplication of slice
			for (unsigned int slice = 1; slice < subdivisions; slice++) {
				offset_copy_to_v.push_back(*start + slice_distance * slice);
			}
		}
		
		// Create copies of the obj defined on the new offset list
		auto output = create_copies(obj_define, offset_copy_to_v);

		return output;
	}

	// Divides the space in between each obj pair in obj_v then creates objects that segment it
	// The object created will be automatically determined by copy_before
	// copy_prev defines if the object created copies the previous or next object
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_subdivision(
		osu_object_v<T> const* obj_v,
		unsigned int subdivisions, bool copy_prev = true, bool include_with = false) {
		std::vector<double> offset_unq_v = obj_v->get_offset_v(true);
		osu_object_v<T> output = osu_object_v<T>();

		// As multiple objects can have the same offset, we want to make sure that subdivisions 
		// are not created in between objects of the same offset
		// To solve this, we create a offset vector that we reference with our object vector
		// We then create objects that take a subdivision of the next offset instead of object

		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}

		auto offset_unq_v_it = offset_unq_v.begin();

		for (auto obj : *obj_v) {

			// This is true when there are no more objects in the offset, so we add 1 to it
			// It is guaranteed to have an object after this, so we do not need to verify again
			if (obj.get_offset() != *offset_unq_v_it) {
				offset_unq_v_it++;

				// In the case where a pair cannot happen after increment
				if (offset_unq_v_it + 1 == offset_unq_v.end()) {
					if (include_with) {
						output.push_back(obj_v->back());
					}
					break;
				}
			}

			// We create a offset_pair to use on the other variant of create_copies_by_subdivision
			std::vector<double> offset_pair = {
				*offset_unq_v_it, // start 
				*(offset_unq_v_it + 1) // end
			};

			output.push_back(
				*create_copies_by_subdivision(offset_pair, obj, subdivisions, include_with));

			// We need to remove the last element as the next pair's first element will overlap
			// This only applies to include_with true as we utilize the overload
			if (include_with) {
				output.pop_back();
			}
		}
		
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Creates a object in between each offset pair in offset_v, placement is determined by relativity
	// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_relative_difference(
		const std::vector<double> offset_v,
		const T obj_define,
		double relativity = 0.5, bool include_with = false) {
		// We create a vector of doubles that we want objects to be created on, then we use
		// create_copies function to duplicate them

		if (offset_v.size() <= 1) {
			throw reamber_exception("offset_v size must be at least 2 for the function to work");
		}

		std::vector<double> offset_copy_to_v = include_with ? offset_v : std::vector<double>();

		for (auto start = offset_v.begin(); start + 1 != offset_v.end(); start ++) {
			double offset_relative_delta = (*(start + 1) - *start) * relativity;
			offset_copy_to_v.push_back(*start + offset_relative_delta);
		}

		return create_copies(obj_define, offset_copy_to_v);
	}

	// Creates a object in between each obj pair in obj_v, placement is determined by relativity
	// If relativity is 0.25, the obj will be created 25% in between obj pairs, closer to the first
	// The object created will be defined by the user
	// include_with defines if the created objects exports alongside the original
	template <typename T>
	std::shared_ptr<osu_object_v<T>> create_copies_by_relative_difference(
		osu_object_v<T> const* obj_v,
		double relativity = 0.5, bool copy_prev = true, bool include_with = false) {

		if (obj_v->size() <= 1) {
			throw reamber_exception("obj_v size must be at least 2 for the function to work");
		}

		std::vector<double> offset_unq_v = obj_v->get_offset_v(true);
		osu_object_v<T> output = osu_object_v<T>();

		// As multiple objects can have the same offset, we want to make sure that subdivisions 
		// are not created in between objects of the same offset
		// To solve this, we create a offset vector that we reference with our object vector
		// We then create objects that take a subdivision of the next offset instead of object

		auto offset_unq_v_it = offset_unq_v.begin();

		for (auto obj : *obj_v) {

			// This is true when there are no more objects in the offset, so we add 1 to it
			// It is guaranteed to have an object after this, so we do not need to verify again
			if (obj.get_offset() != *offset_unq_v_it) {
				offset_unq_v_it++;

				// In the case where a pair cannot happen after increment
				if (offset_unq_v_it + 1 == offset_unq_v.end()) {
					if (include_with) {
						output.push_back(obj_v->back());
					}
					break;
				}
			}

			// We create a offset_pair to use on the other variant of create_copies_by_subdivision
			std::vector<double> offset_pair = {
				*offset_unq_v_it, // start 
				*(offset_unq_v_it + 1) // end
			};

			output.push_back(
				*lib_functions::create_copies_by_relative_difference(offset_pair, obj, relativity, include_with));

			// We need to remove the last element as the next pair's first element will overlap
			// This only applies to include_with true as we utilize the overload
			if (include_with) {
				output.pop_back();
			}
		}
		
		return std::make_shared<osu_object_v<T>>(output);
	}

	// Automatically creates tps to counteract bpm line scroll speed manipulation
	// include_with defines if the created tps exports alongside the original
	timing_point_v create_normalize(timing_point_v tp_v, const double &reference, bool include_with = false) {
		timing_point_v output = include_with ? tp_v : timing_point_v();
		tp_v = tp_v.get_bpm_only();

		if (tp_v.size() == 0) {
			throw reamber_exception("tp_v BPM size is 0");
		}

		for (auto tp : tp_v) {
			tp.set_value(reference / tp.get_value());
			tp.set_is_sv(true);
			output.push_back(tp);
		}

		return output;
	}

	// Used to find the limits of create_basic_stutter
	// [0] is min, [1] is max
	std::vector<double> create_basic_stutter_threshold_limits(
		double initial, double average, double threshold_min = 0.1, double threshold_max = 10.0) {

		// init * thr + thr_ * ( 1 - thr ) = ave
		// init * thr + thr_ - thr * thr_ = ave
		// init * thr - thr * thr_ = ave - thr_
		// thr * ( init - thr_ ) = ave - thr_
		// thr = ( ave - thr_ ) / ( init - thr_ )

		double thr_1 = (average - threshold_min) / (initial - threshold_min);
		double thr_2 = (average - threshold_max) / (initial - threshold_max);

		std::vector<double> output;
		if (thr_1 < thr_2) {
			output.push_back(thr_1);
			output.push_back(thr_2);
		}
		else {
			output.push_back(thr_2);
			output.push_back(thr_1);
		}
		return output;
	}

	// Used to find the limits of create_basic_stutter
	// [0] is min, [1] is max
	std::vector<double> create_basic_stutter_init_limits(
		double threshold, double average, double threshold_min = 0.1, double threshold_max = 10.0) {

		// init * thr + thr_ * ( 1 - thr ) = ave
		// init * thr + thr_ - thr * thr_ = ave
		// init * thr = ave - thr_ + thr * thr_
		// init = thr_ + [( ave - thr_ ) / thr] 
		
		double init_1 = threshold_min + ((average - threshold_min) / threshold);
		double init_2 = threshold_max + ((average - threshold_max) / threshold);

		std::vector<double> output;
		if (init_1 < init_2) {
			output.push_back(init_1);
			output.push_back(init_2);
		}
		else {
			output.push_back(init_2);
			output.push_back(init_1);
		}
		return output;
	}

	// Creates a simple Act - CounterAct - Normalize movement
	// Stutter creation will chain on more than 2 offsets
	timing_point_v create_basic_stutter(const std::vector<double> &offset_v, double initial,
		double threshold, double average = 1.0, bool is_bpm = false) {

		if (offset_v.size() == 0) {
			throw reamber_exception("tp_v size is 0");
		}
		else if (threshold > 1 || threshold < 0) {
			throw reamber_exception("threshold must be in between 0 and 1" );
		}

		// We will do 2 create_copies calls,
		// 1) initial -> offset_v
		// 2) threshold_tp -> offset_threshold_v

		timing_point_v output = timing_point_v();

		std::vector<double> offset_threshold_v = {};

		for (auto it = offset_v.begin(); it + 1 < offset_v.end(); it ++) {
			offset_threshold_v.push_back((*(it + 1) - *it) * threshold + *it);
		}

		double threshold_tp = (average - (threshold * initial)) / (1 - threshold);

		// 1) initial -> offset_v

		timing_point tp;
		tp.set_is_bpm(is_bpm);
		tp.set_value(initial);

		output.push_back(*create_copies(tp, offset_v, true));

		output[offset_v.size() - 1].set_value(average); // We use the last offset as a normalizer
		
		// 2) threshold_tp -> offset_threshold_v

		tp.set_value(threshold_tp);

		output.push_back(*create_copies(tp, offset_threshold_v, true));

		std::sort(output.begin(), output.end());

		return output;
	}


};
