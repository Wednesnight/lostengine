#! /bin/sh

top_srcdir="$1"
top_builddir="$2"

if test x"$top_builddir" = x -o x"$top_srcdir" = x ; then
    echo "usage: $0 <top_srcdir> <top_builddir>" >&2
    exit 1
fi

echo "Processing coverage files..."
have_gcda=
# create gcov files
for gcda_file in $(find "$top_builddir" -type f -name \*.gcda) ; do
    echo "  Processing '$gcda_file'..."
    have_gcda=1
    gcda_dir=$(dirname "$gcda_file")
    trunc_dir=$(echo "$gcda_dir" | sed 's:/\.libs$::g')
    remain_dir=".$(echo "$gcda_dir" | sed "s:$trunc_dir::g")"
    gcda_filename=$(echo $gcda_file | sed "s:$gcda_dir/::g")

    cwd=$(pwd)
    cd "$trunc_dir"
    gcov -p -o "$remain_dir" "$gcda_filename" >/dev/null
    mkdir -p "$remain_dir/coverage/$gcda_filename"
    # collect and prune gcov files
    for gcov_file in $(find . -type f -name \*.gcov) ; do
        # skip files collected into a coverage subdir
        if echo "$gcov_file" | grep -q "/coverage/" ; then
            continue
        fi
        # delete files that start with a '#' character - that's gcov's conversion
        # of a slash, and a certain hint that the file isn't part of the current
        # project.
        fname=$(echo "$gcov_file" | sed "s:^$(dirname "$gcov_file")::g")
        if echo "$fname" | grep -q '^/#' ; then
            rm -f "$gcov_file"
            continue
        fi
        mv "$gcov_file" "$remain_dir/coverage/$gcda_filename"
    done
    cd "$cwd"
done

if test -z "$have_gcda" ; then
    echo "No coverage files found! You may need to execute some program (such as a testsuite) to generate them." >&2
    exit 0
fi

echo "Collecting coverage files..."
rm -rf "$top_builddir/coverage"
mkdir -p "$top_builddir/coverage"
for cov_dir in $(find "$top_builddir" -type d -name \*.gcda) ; do
    echo "  Processing '$cov_dir'..."
    cov_dir_name=$(echo "$cov_dir" | sed "s:^$(dirname "$cov_dir")/::g")
    cov_dir_name=$(echo "$cov_dir_name" | sed 's:\.gcda$::g')
    target="$top_builddir/coverage/$cov_dir_name"
    mv "$cov_dir" "$target"
done

echo "Cleaning up..."
for d in $(find "$top_builddir" -type d -name coverage) ; do
    if ls $d/* >/dev/null 2>&1 ; then
        # found files, keep directory
        true
    else
        rm -rf "$d"
    fi
done

echo "Done! Look for the results in '$top_builddir/coverage'."
