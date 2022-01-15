package Hw2

import fastparse._
import MultiLineWhitespace._
import scala.collection.immutable.HashMap 

sealed trait Val
case class IntVal(n: Int) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(v: Var, expr: Expr, env: Env) extends Val
case class RecProcVal(fv: Var, av: Var, body: Expr, env: Env) extends Val

case class Env(hashmap: HashMap[Var,Val]) {
  def apply(variable: Var): Val = hashmap(variable)
  def exists(v: Var): Boolean = 
    hashmap.exists((a: (Var, Val)) => a._1 == v)
  def add(v: Var, value: Val) = Env(hashmap + (v -> value))
  
}

sealed trait Program
sealed trait Expr extends Program
case class Const(n: Int) extends Expr
case class Var(s: String) extends Expr
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Equal(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class Let(name: Var, value: Expr, body: Expr) extends Expr
case class Paren(expr: Expr) extends Expr
case class Proc(v: Var, expr: Expr) extends Expr
case class PCall(ftn: Expr, arg: Expr) extends Expr
case class LetRec(fname: Var, aname: Var, fbody: Expr, ibody: Expr) extends Expr

sealed trait IntExpr
case class IntConst(n: Int) extends IntExpr
case object IntVar extends IntExpr
case class IntAdd(l: IntExpr, r: IntExpr) extends IntExpr
case class IntSub(l: IntExpr, r: IntExpr) extends IntExpr
case class IntMul(l: IntExpr, r: IntExpr) extends IntExpr
case class IntSigma(f: IntExpr, t: IntExpr, b: IntExpr) extends IntExpr
case class IntPi(f: IntExpr, t: IntExpr, b: IntExpr) extends IntExpr
case class IntPow(b: IntExpr, e: IntExpr) extends IntExpr



package object Hw2 {

  

}

object IntInterpreter {
  def evalInt(expr: IntExpr, env: Option[Int]): Int = expr match{
    case IntConst(n) => n
    case IntVar => env match{
      case Some(n) => n
      case None => throw new Exception("1")
    }
    case IntAdd(l, r) => (evalInt(l, env), evalInt(r, env)) match{
      case (x: Int, y: Int) => evalInt(IntConst(x + y), env)
      case _ => throw new Exception("Type Error")
    }
    case IntSub(l, r) => (evalInt(l, env), evalInt(r, env)) match{
      case (x: Int, y: Int) => evalInt(IntConst(x - y), env)
      case _ => throw new Exception("Type Error")
    }
    case IntMul(l, r) => (evalInt(l, env), evalInt(r, env)) match{
      case (x: Int, y: Int) => evalInt(IntConst(x * y), env)
      case _ => throw new Exception("Type Error")
    }
    case IntPow(b, e) => evalInt(e,env) match{
      case 0 => 1
      case _ => evalInt(IntMul(b, IntPow(b, IntConst(evalInt(IntSub(e,IntConst(1)), env)))),env)
    }
    case IntSigma(f, t, b) =>{
      if(evalInt(f, env) <= evalInt(t, env)){
        evalInt(IntSigma(IntConst(evalInt(IntAdd(f, IntConst(1)), env)), t, b), env) + evalInt(b, Option(evalInt(f, env)))
      }
      else{
        0
      }
    }
    case IntPi(f, t, b) =>{
      if(evalInt(f, env) <= evalInt(t, env)){
        evalInt(IntPi(IntConst(evalInt(IntAdd(f, IntConst(1)), env)), t, b), env) * evalInt(b, Option(evalInt(f, env)))
      }
      else{
        1
      }
    }
  }
  
  def apply(s: String): Int = {
    val parsed = IntParser(s)
    evalInt(parsed, None)
  }
}

object LetRecInterpreter {
  
  def eval(env: Env, expr: Expr): Val = expr match{
    case Const(n) => IntVal(n)
    case Var(s) => {
      if(env.exists(Var(s))){
        env.apply(Var(s))
      }
      else{
        throw new Exception("1")
      }
    }
    case Add(l, r) => (eval(env, l), eval(env, r)) match{
      case(a: IntVal, b: IntVal) => IntVal(a.n + b.n)
      case _ => throw new Exception("Type error")
    }
    case Sub(l, r) => (eval(env, l), eval(env, r)) match{
      case(a: IntVal, b: IntVal) => IntVal(a.n - b.n)
      case _ => throw new Exception("Type error")
    }
    case Equal(l, r) => (eval(env, l), eval(env, r)) match{
      case(a: IntVal, b: IntVal) => {
        if(a.n == b.n)
          BoolVal(true)
        else
          BoolVal(false)
      }
      case(x: BoolVal, y: BoolVal) => {
        if(x.b == y.b)
          BoolVal(true)
        else{
          BoolVal(false)
        }
      }
      case _ => throw new Exception("Type error")
    }
    case Iszero(c) => eval(env, c) match{
      case x: IntVal => BoolVal(x.n==0)
      case _ => BoolVal(false)
    }
    case Ite(c, t, f) => eval(env, c) match{
      case v: BoolVal => {
        if(v.b)
          eval(env, t)
        else
          eval(env, f)
      }
      case _ => throw new Exception("Type error")
    }
    case Let(name, value, body) => {
      val env_1 = env.add(name, eval(env, value))
      eval(env_1, body)
    }
    case Paren(expr: Expr) => eval(env, expr)
    case Proc(v, expr) => ProcVal(v, expr, env)
    case PCall(ftn, arg) => (eval(env, ftn), eval(env, arg)) match{
      case (p: ProcVal, i: Val) =>{
        val env_2 = p.env.add(p.v, i)
        eval(env_2, p.expr) 
      }
      case (r: RecProcVal, i: Val) =>{
        val env_4 = r.env.add(r.av, i)
        val env_5 = env_4.add(r.fv, RecProcVal(r.fv, r.av, r.body, env_4))
        eval(env_5, r.body)
      }
      case _ => throw new Exception
    }
    case LetRec(fname, aname, fbody, ibody) =>  {
      val env_3 = env.add(fname, RecProcVal(fname, aname, fbody, env))
      eval(env_3, ibody)
    }
    case _ => throw new Exception
  }  
    
  
  def apply(program: String): Val = {
    val parsed = LetRecParserDriver(program)
    eval(Env(new HashMap[Var,Val]()), parsed)
  }

}

object LetRecToString {
  def apply(expr: Expr): String = expr match{
    case Const(n) => s"${n}"
    case Var(s) => s 
    case Add(l, r) => s"${apply(l)} + ${apply(r)}"
    case Sub(l, r) => s"${apply(l)} - ${apply(r)}"
    case Equal(l, r) => s"${apply(l)} == ${apply(r)}"
    case Iszero(c) => s"iszero ${apply(c)}"
    case Ite(c, t, f) => s"if ${apply(c)} then ${apply(t)} else ${apply(f)}"
    case Let(name, value, body) => s"let ${apply(name)} = ${apply(value)} in ${apply(body)}"
    case Paren(ex) => s"(${apply(ex)})"
    case Proc(v, ex) => s"proc ${apply(v)} ${apply(ex)}"
    case PCall(ftn, arg) => s"${apply(ftn)} ${apply(arg)}"
    case LetRec(fname, aname, fbody, ibody) => s"letrec ${apply(fname)}(${apply(aname)}) = ${apply(fbody)} in ${apply(ibody)}"
  }
}

object Hw2App extends App {
  
  println("Hello from Hw2!")

  val int_prog = """x + 1"""
  val parsed = IntParser(int_prog)
  println(parsed)


}
