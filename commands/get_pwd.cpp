string get_cwd()
{
    char buffer[1024];
    if (getcwd(buffer, 1024) == nullptr) {
        return "";
    }
    return std::string(buffer);
}