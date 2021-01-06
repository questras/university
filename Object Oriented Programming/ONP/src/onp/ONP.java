package onp;


class TooComplexExpressionException extends Exception {
    public TooComplexExpressionException() {
        super("Too complex expression.");
    }
}

class TooManyOperatorsException extends Exception {
    public TooManyOperatorsException() {
        super("Too many operators.");
    }
}

class TooFewOperatorsException extends Exception {
    public TooFewOperatorsException() {
        super("Too few operators.");
    }
}

class UnknownSymbolException extends Exception {
    public UnknownSymbolException() {
        super("Unknown symbol.");
    }
}

public class ONP {

    private boolean isOperator(String s) {
        return s.equals("+") || s.equals("*");
    }

    private int calculate(int a, int b, String operator) {
        if (operator.equals("+")) {
            return a + b;
        }
        else if (operator.equals("*")) {
            return a * b;
        }

        return 0;
    }

    private void parseOperator(Stack stack, String operator)
            throws TooManyOperatorsException,
            TooComplexExpressionException {

        int a, b;

        try {
            a = stack.pop();
            b = stack.pop();
        } catch (EmptyException e) {
            throw new TooManyOperatorsException();
        }

        try {
            stack.add(calculate(a, b, operator));
        } catch (OverflowException e) {
            throw new TooComplexExpressionException();
        }
    }

    private void parseNumber(Stack stack, String number)
            throws UnknownSymbolException,
            TooComplexExpressionException {

        int converted;

        try {
            converted = Integer.parseInt(number);
        } catch (NumberFormatException e) {
            throw new UnknownSymbolException();
        }

        try {
            stack.add(converted);
        } catch (OverflowException e) {
            throw new TooComplexExpressionException();
        }
    }

    public int oblicz(String[] data)
            throws TooManyOperatorsException,
            TooComplexExpressionException,
            UnknownSymbolException,
            TooFewOperatorsException {

        Stack stack = new Stack();

        for (String symbol : data) {
            if (this.isOperator(symbol)) {
                this.parseOperator(stack, symbol);
            }
            else {
                this.parseNumber(stack, symbol);
            }
        }

        if (stack.getSize() > 1) {
            throw new TooFewOperatorsException();
        }
        else {
            try {
                return stack.pop();
            } catch (EmptyException e) {
                // Possible only when [data] is initially empty.
                return 0;
            }
        }
    }
}
