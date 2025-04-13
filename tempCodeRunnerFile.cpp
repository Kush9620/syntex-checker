void checkForErrors(const std::vector<Line>& lines) {
    bool foundError = false;

    // Regex to detect access specifier without ':' inside class body
    std::regex accessSpecifierWithoutColon(R"(\b(public|private|protected)\b(?!\s*:))");

    for (const auto& line : lines) {
        // Check for standalone access specifier without a colon in the class body
        if (std::regex_search(line.text, accessSpecifierWithoutColon)) {
            // Found an access specifier missing a colon
            std::cout << "Error: Access specifier (public/private/protected) should be followed by ':' ---> line no: " 
                      << line.lineno << std::endl;
            foundError = true;
        }
    }

    if (!foundError) {
        std::cout << "No syntax errors found." << std::endl;
    }
}