using Documenter

makedocs(
    sitename="Quibble",
    authors="James Schloss (Leios) and contributors",
    pages = [
        "General Information" => "index.md",
    ],
)

deploydocs(;
    repo="github.com/leios/quibble",
)
