#! /bin/bash

OLD="$1"
NEW="$2"

# add space after nonterminal name and remove tabs
sed -E 's:^([^ ]+):\1 :g' "$OLD" | tr -d "\t" > TMP_OLD
sed -E 's:^([^ ]+):\1 :g' "$NEW" | tr -d "\t" > TMP_NEW

diff TMP_OLD TMP_NEW

rm TMP_OLD TMP_NEW
