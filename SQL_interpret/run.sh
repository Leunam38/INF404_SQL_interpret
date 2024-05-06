make interpreteur
echo "TESTS CORRECTS"
echo "-----------------------------------"
for file in Tests/Tests_correct/*.txt
do
    if [ -f "$file" ] 
        then
            echo -e "Running interpreteur on $file (valide)\nrequete:"
            cat $file
            echo -e "\nRésultat:"
            ./interpreteur $file
            echo "-----------------------------------"
        fi
done
echo "TESTS INCORRECTS"
echo "-----------------------------------"
for file in Tests/Tests_incorrect/*.txt
do
    if [ -f "$file" ] 
        then
            echo -e "Running interpreteur on $file (invalide)\nrequete:"
            cat $file
            echo -e "\nRésultat:"
            ./interpreteur $file
            echo "-----------------------------------"
        fi
done
#make clean