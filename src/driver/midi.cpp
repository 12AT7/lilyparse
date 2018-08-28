midi::pitch pitch::get_midi() const
{
    // Compute the actual pitch number, using the MIDI convention C4=60.
    // As this reduction discards all subtlety about enharmonics, it is
    // used only in specific places that need this representation (like
    // MIDI note_on/note_off and geometry calculations).  There is no way
    // to go backwards, from int to pitch, because of the enharmonic
    // ambiguity.

    using pc = stan::pitchclass;
    static const std::map<pitchclass, std::uint8_t> midi{ { pc::cff, 58 },
                                                          { pc::cf, 59 },
                                                          { pc::c, 60 },
                                                          { pc::cs, 61 },
                                                          { pc::css, 62 },
                                                          { pc::dff, 60 },
                                                          { pc::df, 61 },
                                                          { pc::d, 62 },
                                                          { pc::ds, 63 },
                                                          { pc::dss, 64 },
                                                          { pc::eff, 62 },
                                                          { pc::ef, 63 },
                                                          { pc::e, 64 },
                                                          { pc::es, 65 },
                                                          { pc::ess, 66 },
                                                          { pc::fff, 63 },
                                                          { pc::ff, 64 },
                                                          { pc::f, 65 },
                                                          { pc::fs, 66 },
                                                          { pc::fss, 67 },
                                                          { pc::gff, 65 },
                                                          { pc::gf, 66 },
                                                          { pc::g, 67 },
                                                          { pc::gs, 68 },
                                                          { pc::gss, 69 },
                                                          { pc::aff, 67 },
                                                          { pc::af, 68 },
                                                          { pc::a, 69 },
                                                          { pc::as, 70 },
                                                          { pc::ass, 71 },
                                                          { pc::bff, 69 },
                                                          { pc::bf, 70 },
                                                          { pc::b, 71 },
                                                          { pc::bs, 72 },
                                                          { pc::bss, 73 } };

    return midi::pitch(
        midi.at(pitchclass_) + (static_cast<std::uint8_t>(octave_) - 4) * 12);
}
