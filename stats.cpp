/*
    SPDX-FileCopyrightText: 2001-2013 Evan Teran <evan.teran@gmail.com>
    SPDX-FileCopyrightText: 1996-2000 Bernd Johannes Wuebben <wuebben@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "stats.h"

//------------------------------------------------------------------------------
// Name: KStats
// Desc: constructor
//------------------------------------------------------------------------------
KStats::KStats() = default;

//------------------------------------------------------------------------------
// Name: ~KStats
// Desc: destructor
//------------------------------------------------------------------------------
KStats::~KStats() = default;

//------------------------------------------------------------------------------
// Name: clearAll
// Desc: empties the data set
//------------------------------------------------------------------------------
void KStats::clearAll()
{
    data_.clear();
}

//------------------------------------------------------------------------------
// Name: enterData
// Desc: adds an item to the data set
//------------------------------------------------------------------------------
void KStats::enterData(const KNumber &data)
{
    data_.push_back(data);
}

//------------------------------------------------------------------------------
// Name: clearLast
// Desc: removes the last item from the data set
//------------------------------------------------------------------------------
void KStats::clearLast()
{
    if (!data_.isEmpty()) {
        data_.pop_back();
    }
}

//------------------------------------------------------------------------------
// Name: sum
// Desc: calculates the SUM of all values in the data set
//------------------------------------------------------------------------------
KNumber KStats::sum() const
{
    KNumber result = KNumber::Zero;
    for (const KNumber &x : std::as_const(data_)) {
        result += x;
    }

    return result;
}

//------------------------------------------------------------------------------
// Name: median
// Desc: calculates the MEDIAN of all values in the data set
//------------------------------------------------------------------------------
KNumber KStats::median()
{
    KNumber result = KNumber::Zero;
    size_t index;

    unsigned int bound = count();

    if (bound == 0) {
        error_flag_ = true;
        return KNumber::Zero;
    }

    if (bound == 1)
        return data_.at(0);

    // need to copy data_-list, because sorting afterwards
    QVector<KNumber> tmp_data(data_);
    std::sort(tmp_data.begin(), tmp_data.end());

    if (bound & 1) { // odd
        index = (bound - 1) / 2 + 1;
        result = tmp_data.at(index - 1);
    } else { // even
        index = bound / 2;
        result = ((tmp_data.at(index - 1)) + (tmp_data.at(index))) / KNumber(2);
    }

    return result;
}

//------------------------------------------------------------------------------
// Name: std_kernel
// Desc: calculates the STD Kernel of all values in the data set
//------------------------------------------------------------------------------
KNumber KStats::std_kernel()
{
    KNumber result = KNumber::Zero;
    const KNumber mean_value = mean();

    if (mean_value.type() != KNumber::TYPE_ERROR) {
        for (const KNumber &x : std::as_const(data_)) {
            result += (x - mean_value) * (x - mean_value);
        }
    }

    return result;
}

//------------------------------------------------------------------------------
// Name: sum_of_squares
// Desc: calculates the SUM of all values in the data set (each squared)
//------------------------------------------------------------------------------
KNumber KStats::sum_of_squares() const
{
    KNumber result = KNumber::Zero;

    for (const KNumber &x : std::as_const(data_)) {
        result += (x * x);
    }

    return result;
}

//------------------------------------------------------------------------------
// Name: mean
// Desc: calculates the MEAN of all values in the data set
//------------------------------------------------------------------------------
KNumber KStats::mean()
{
    if (data_.isEmpty()) {
        error_flag_ = true;
        return KNumber::Zero;
    }

    return (sum() / KNumber(count()));
}

//------------------------------------------------------------------------------
// Name: std
// Desc: calculates the STANDARD DEVIATION of all values in the data set
//------------------------------------------------------------------------------
KNumber KStats::std()
{
    if (data_.isEmpty()) {
        error_flag_ = true;
        return KNumber::Zero;
    }

    return (std_kernel() / KNumber(count())).sqrt();
}

//------------------------------------------------------------------------------
// Name: sample_std
// Desc: calculates the SAMPLE STANDARD DEVIATION of all values in the data set
//------------------------------------------------------------------------------
KNumber KStats::sample_std()
{
    KNumber result = KNumber::Zero;

    if (count() < 2) {
        error_flag_ = true;
        return KNumber::Zero;
    }

    result = (std_kernel() / KNumber(count() - 1)).sqrt();

    return result;
}

//------------------------------------------------------------------------------
// Name: count
// Desc: returns the amount of values in the data set
//------------------------------------------------------------------------------
int KStats::count() const
{
    return data_.size();
}

//------------------------------------------------------------------------------
// Name: error
// Desc: returns the error state AND clears it
//------------------------------------------------------------------------------
bool KStats::error()
{
    bool value = error_flag_;
    error_flag_ = false;
    return value;
}
