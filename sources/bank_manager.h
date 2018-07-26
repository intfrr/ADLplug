//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "definitions.h"
#include "adl/instrument.h"
#include "utility/counting_bitset.h"
#include <array>
#include <bitset>
class AdlplugAudioProcessor;

class Bank_Manager {
public:
    explicit Bank_Manager(AdlplugAudioProcessor &proc, Generic_Player &pl);
    void clear_banks(bool notify);

    void mark_everything_for_notification();
    void mark_slots_for_notification();
    void send_notifications();
    void send_measurement_requests();

    bool load_global_parameters(const Instrument_Global_Parameters &gp, bool notify);

    bool load_program(const Bank_Id &id, unsigned program, const Instrument &ins, unsigned flags);
    bool find_program(const Bank_Id &id, unsigned program, Instrument &ins);

    bool load_measurement(const Bank_Id &id, unsigned program, const Instrument &ins, unsigned kon, unsigned koff, bool notify);

    void rename_bank(const Bank_Id &id, const char *name, bool notify);

    enum {
        LP_Notify          = 1,
        LP_NeedMeasurement = 2,
        LP_KeepName        = 4,
    };

private:
    void initialize_all_banks();

    unsigned find_slot(const Bank_Id &id);
    unsigned ensure_find_slot(const Bank_Id &id);
    unsigned find_empty_slot();

    struct Bank_Info;

    bool emit_slots();
    bool emit_global_parameters();
    bool emit_notification(const Bank_Info &info, unsigned program);
    bool emit_measurement_request(const Bank_Info &info, unsigned program);

    AdlplugAudioProcessor &proc_;
    Generic_Player &pl_;

    struct Bank_Info {
        Bank_Id id;
        Bank_Ref bank;
        counting_bitset<128> used;
        counting_bitset<128> to_notify;
        counting_bitset<128> to_measure;
        char bank_name[32];
        char ins_names[128 * 32];
        explicit operator bool() const noexcept
            { return bool(id); }
    };
    std::array<Bank_Info, bank_reserve_size> bank_infos_;

    bool global_parameters_notify_flag_ = false;
    bool slots_notify_flag_ = false;
};
