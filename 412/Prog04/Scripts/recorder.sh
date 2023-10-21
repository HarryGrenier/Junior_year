#!/bin/bash

commands=()

while true; do
    echo -n "Enter a command: "
    read cmd

    # Check for file redirection in the command
    if echo "$cmd" | grep -E '>|>>' &>/dev/null; then
        echo "File redirection detected. Please enter another command."
        continue
    fi

    # Execute the command
    eval "$cmd"

    # Prompt the user for an action
    echo -n "Record (R), Discard (D), or Stop recording (STOP)? "
    read action

    case $action in
        R|r) commands+=("$cmd") ;;
        D|d) ;;
        STOP|stop) 
            echo -n "Enter the path to save the commands: "
            read filepath
            printf "%s\n" "${commands[@]}" > "$filepath"
            exit 0
            ;;
        *) echo "Invalid choice. Discarding the command."
    esac
done
