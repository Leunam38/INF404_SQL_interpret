make test_syntaxique
echo "TESTS CORRECTS"
echo "-----------------------------------"
for file in Tests/Tests_correct/*.txt
do
    if [ -f "$file" ] 
        then
            echo -e "Running test_syntaxique on $file (valide)\nrequete:"
            cat $file
            echo -e "\nRésultat:"
            ./test_syntaxique $file
            echo "-----------------------------------"
        fi
done
echo "TESTS INCORRECTS"
echo "-----------------------------------"
for file in Tests/Tests_incorrect/*.txt
do
    if [ -f "$file" ] 
        then
            echo -e "Running test_syntaxique on $file (valide)\nrequete:"
            cat $file
            echo -e "\nRésultat:"
            ./test_syntaxique $file
            echo "-----------------------------------"
        fi
done
#make clean