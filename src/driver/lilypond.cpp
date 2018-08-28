std::string to_string(const pitch &p)
{
    std::string lily = to_string(p.pitchclass_);
    auto rawoctave = static_cast<std::uint8_t>(p.octave_);
    std::copy_n(",,,,", 4 - rawoctave, std::back_inserter(lily));
    std::copy_n("''''", rawoctave - 4, std::back_inserter(lily));
    return lily;
}
