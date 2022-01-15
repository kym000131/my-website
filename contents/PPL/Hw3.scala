package hw3

import scala.collection.immutable.HashMap 
import hw3._


package object hw3 {
  type Env = HashMap[Var,Val]
  type Loc = Int
  
}

case class Mem(m: HashMap[Loc,Val], top: Loc) {
  def add(value: Val) = Mem(m+(top+1 -> value), top+1)
  def edit(l: Loc, value: Val) = Mem(m + (l -> value), top)
}

sealed trait Val
case class IntVal(n: Int) extends Val
case class IntListVal(n: List[IntVal]) extends Val
case class BoolVal(b: Boolean) extends Val
case class ProcVal(v: Var, expr: Expr, env: Env) extends Val
case class RecProcVal(fv: Var, av: Var, body: Expr, env: Env) extends Val
case class LocVal(l: Loc) extends Val


sealed trait Program
sealed trait Expr extends Program
case class ConstI(n: Int) extends Expr
case class ConstB(n: Boolean) extends Expr
case class ConstIL(n: List[IntVal]) extends Expr
case class Var(s: String) extends Expr
case class Add(l: Expr, r: Expr) extends Expr
case class Sub(l: Expr, r: Expr) extends Expr
case class Mul(l: Expr, r: Expr) extends Expr
case class Div(l: Expr, r: Expr) extends Expr
case class Rem(l: Expr, r: Expr) extends Expr
case class Cons(l: Expr, r: Expr) extends Expr
case class GTExpr(l: Expr, r: Expr) extends Expr
case class GEQExpr(l: Expr, r: Expr) extends Expr
case class Iszero(c: Expr) extends Expr
case class Ite(c: Expr, t: Expr, f: Expr) extends Expr
case class ValExpr(name: Var, value: Expr, body: Expr) extends Expr
case class VarExpr(name: Var, value: Expr, body: Expr) extends Expr
case class Proc(v: Var, expr: Expr) extends Expr
case class DefExpr(fname: Var, aname: Var, fbody: Expr, ibody: Expr) extends Expr
case class Asn(v: Var, e: Expr) extends Expr
case class Paren(expr: Expr) extends Expr
case class Block(f: Expr, s: Expr) extends Expr
case class PCall(ftn: Expr, arg: Expr) extends Expr







object MiniScalaInterpreter {

  case class Result(v: Val, m: Mem)
  case class UndefinedSemantics(msg: String = "", cause: Throwable = None.orNull) extends Exception("Undefined Semantics: " ++ msg, cause)
  
  
  def eval(env: Env, mem: Mem, expr: Expr): Result = expr match{
    case ConstI(n) => Result(IntVal(n), mem)
    case ConstB(b) => Result(BoolVal(b), mem)
    case ConstIL(n) => Result(IntListVal(n), mem)
    case Var(s) => {
      if(env.exists((a: (Var, Val)) => a._1 == Var(s))){
        env(Var(s)) match {
          case LocVal(l) => Result(mem.m(l), mem)
          case _ => Result(env(Var(s)), mem)
        }
      }
      else{
        throw new UndefinedSemantics(s"message ${s}")
      }
    } 
    case Add(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem2) => eval(env, mem2, r) match{
        case Result(IntVal(n2), mem3) => Result(IntVal(n1+n2), mem3)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Sub(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem2) => eval(env, mem2, r) match{
        case Result(IntVal(n2), mem3) => Result(IntVal(n1-n2), mem3)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Mul(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem2) => eval(env, mem2, r) match{
        case Result(IntVal(n2), mem3) => Result(IntVal(n1*n2), mem3)
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Div(l, r) => eval(env, mem, r) match{
      case Result(IntVal(n1), mem2) => {
        if(n1 == 0){
          throw new UndefinedSemantics(s"message ${r}")
        }
        else{
          eval(env, mem2, l) match{
            case Result(IntVal(n2), mem3) => Result(IntVal(n2/n1), mem3)
            case _ => throw new UndefinedSemantics(s"message ${l}")
          }
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${r}")
    }
    case Rem(l, r) => eval(env, mem, r) match{
      case Result(IntVal(n1), mem2) => {
        if(n1 == 0){
          throw new UndefinedSemantics(s"message ${r}")
        }
        else {
          eval(env, mem2, l) match{
            case Result(IntVal(n2), mem3) => Result(IntVal(n2%n1), mem3)
            case _ => throw new UndefinedSemantics(s"message ${l}")
          }
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${r}")
    }
    case Cons(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem2) => eval(env, mem2, r) match{
        case Result(IntListVal(n2), mem2) => {
          Result(IntListVal(IntVal(n1)::n2), mem)
        }
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case GTExpr(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem2) => eval(env, mem2, r) match{
        case Result(IntVal(n2), mem3) => {
          if(n1 > n2){
            Result(BoolVal(true), mem3)
          }
          else{
            Result(BoolVal(false), mem3)
          }
        }
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case GEQExpr(l, r) => eval(env, mem, l) match{
      case Result(IntVal(n1), mem2) => eval(env, mem2, r) match{
        case Result(IntVal(n2), mem3) => {
          if(n1 >= n2){
            Result(BoolVal(true), mem3)
          }
          else{
            Result(BoolVal(false), mem3)
          }
        }
        case _ => throw new UndefinedSemantics(s"message ${r}")
      }
      case _ => throw new UndefinedSemantics(s"message ${l}")
    }
    case Iszero(c) => eval(env, mem, c) match{
      case Result(IntVal(n), mem2) =>{
        if(n == 0){
          Result(BoolVal(true), mem2)
        }
        else{
          Result(BoolVal(false), mem2)
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${c}")
    }
    case Ite(c , t , f) => eval(env, mem, c) match {
      case Result(BoolVal(true), mem2) => {
        eval(env, mem2, t)
      }
      case Result(BoolVal(false), mem2) => {
        eval(env, mem2, f)
      }
      case _ => throw new UndefinedSemantics(s"message ${c}")
    }
    case ValExpr(name, value, body) => eval(env, mem, value) match{
      case Result(v, m) => {
        val env2 = env + (name -> v)
        eval(env2, m, body)
      }
      case _ => throw new UndefinedSemantics(s"message ${value}")
    }
    case VarExpr(name, value, body) => eval(env, mem, value) match{
      case Result(v, mem2) => {
        val env2 = env + (name -> LocVal(mem2.top + 1))
        val m3 = mem2.add(v)
        eval(env2, m3, body)
      }
      case _ => throw new UndefinedSemantics(s"message ${value}")
    }
    case Proc(v, expr) => Result(ProcVal(v, expr, env), mem)
    case DefExpr(fname, aname, fbody, ibody) => {
      val env2 = env + (fname -> RecProcVal(fname, aname, fbody, env))
      eval(env2, mem, ibody)
    }
    case Asn(v, expr) => eval(env, mem, expr) match{
      case Result(value, mem1) => {
        if(env.exists((a: (Var, Val)) => a._1 == v)){
          env(v) match{
            case LocVal(l) => {
              val mem2 = mem1.edit(l, value)
              Result(value, mem2)
            }
            case _ => throw new UndefinedSemantics(s"message ${v}")
          }
        }
        else{
          throw new UndefinedSemantics(s"message ${v}")
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${expr}")
    }
    case Paren(expr) => eval(env, mem, expr) match{
      case Result(v, mem2) => {
        Result(v, mem2)
      }
      case _ => throw new UndefinedSemantics(s"message ${expr}")
    }
    case Block(f, r) => eval(env, mem, f) match{
      case Result(v, mem2) => {
        eval(env, mem2, r)
      }
    }
    case PCall(ftn, arg) => eval(env, mem, ftn) match{
      case Result(ProcVal(v, expr, env1), mem1) => eval(env, mem1, arg) match{
        case Result(value, mem2) => {
          val env2 = env1 + (v -> value)
          eval(env2, mem2, expr)
        }
      }
      case Result(RecProcVal(fv, av, body, env1), mem1) => eval(env, mem1, arg) match{
        case Result(value, mem2) => {
          val env2 = env1 + (av -> value)
          val env3 = env2 + (fv -> RecProcVal(fv, av, body, env2))
          eval(env3, mem2, body)
        }
      }
      case _ => throw new UndefinedSemantics(s"message ${ftn}")
    }
  }
  
  def apply(program: String): Val = {
    val parsed = MiniScalaParserDriver(program)
    eval(new Env(), Mem(new HashMap[Loc,Val],0), parsed).v
  }

}


object Hw3App extends App {
  
  println("Hello from Hw3!")

}