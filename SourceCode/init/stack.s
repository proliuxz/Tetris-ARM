;;; Copyright ARM Ltd 2001. All rights reserved.

        AREA    Stacks, DATA, NOINIT

        EXPORT UserStack
        EXPORT SVCStack
        EXPORT UndefStack
        EXPORT IRQStack
        EXPORT AbortStack
        EXPORT FIQStack

UserStack	SPACE	256
SVCStack	SPACE	256	
UndefStack	SPACE	256	
AbortStack	SPACE	256	
IRQStack	SPACE	512	
FIQStack	SPACE	256

        END
